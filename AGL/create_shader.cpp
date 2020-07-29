#include "entity.h"
#include<sstream>

namespace agl {
namespace {
std::pair<GLfloat, GLfloat> getMinMax(std::vector<GLfloat> &vertices, int length, int stride, int offset)
{
    float mn = vertices[offset], mx = vertices[offset];
    for(int i=1; i<length; ++i)
    {
        mn = std::min(mn, vertices[i*stride+offset]);
        mx = std::max(mx, vertices[i*stride+offset]);
    }
    return std::pair<GLfloat, GLfloat>(mn, mx);
}

void setColorComp(const char *comp, glm::vec4 color, Entity *e, std::stringstream &fs)
{
    int l = e->vertices.size() / 3;
    std::pair<GLfloat, GLfloat> mnmx;
    switch(int(color.w))
    {
    case AGL_COLOR_POS2RGB:
        fs << "    vec4 " << comp << " = vec4(";
        for(int i=0; i<3; ++i)
            if(color[i] == -1)
            {
                mnmx = getMinMax(e->vertices, l, 3, i);
                fs << "(pos[" << i << "] - " << mnmx.first << ") * " << 1 / (mnmx.second - mnmx.first) << ", ";
            }
            else
                fs << color[i] << ", ";
        fs << "1);\n";
        break;
    case AGL_COLOR_NORM2RGB:
        fs << "    vec4 " << comp << " = vec4((nrm + 1) * .5, 1);\n";
        break;
    case AGL_COLOR_CHECKER:
        fs << "    pm = mod(";
        for(int i=0; i<3; ++i)
            if(color[i] == -1)
                fs << "+floor(pos[" << i << "])";
        fs << ", 2);\n"
              "    vec4 " << comp << " = vec4(pm, pm, pm, 1);\n";
        break;
    }
}

void setupColors(Material *m, Entity *e, std::stringstream &fs, bool lightsEnabled, bool tex)
{
    if(tex)
        if(lightsEnabled)
            fs << "    vec4 ambient = texture(txtr, uv),\n"
                  "         diffuse = ambient, specular = ambient;\n";
        else
            fs << "    vec4 emission = texture(txtr, uv);\n";
    else
    {
        if(m->ambient.w==AGL_COLOR_CHECKER || m->diffuse.w==AGL_COLOR_CHECKER || m->specular.w==AGL_COLOR_CHECKER || m->emission.w==AGL_COLOR_CHECKER)
            fs << "    float pm;\n";
        if(lightsEnabled)
        {
            if(m->ambient.w < 0)
                setColorComp("ambient", m->ambient, e, fs);
            if(m->diffuse.w < 0)
                setColorComp("diffuse", m->diffuse, e, fs);
            if(m->specular.w < 0)
                setColorComp("specular", m->specular, e, fs);
        }
        if(m->emission.w < 0)
            setColorComp("emission", m->emission, e, fs);
    }
}
}

std::pair<std::string, std::string> Material::createShader(Entity *e, std::vector<Light*> lights)
{
    std::stringstream vs, fs;
    bool norm = !e->normals.empty(),
         tex = !e->uvs.empty() && tex_width>0 && tex_height>0 && tex_channel>0 && texture!=nullptr,
         norm2col = (ambient.w==AGL_COLOR_NORM2RGB || diffuse.w==AGL_COLOR_NORM2RGB || specular.w==AGL_COLOR_NORM2RGB || emission.w==AGL_COLOR_NORM2RGB) && norm;
// ----------vertex shader----------
    vs << "#version " << AGL_GLVERSION_MAJOR << AGL_GLVERSION_MINOR << "0 core\n"
          "layout(location = 0) in vec3 vertexPos;\n";
    if(norm)
        vs << "layout(location = 1) in vec3 normal;\n";
    if(tex)
        vs << "layout(location = " << (norm ? 2 : 1) << ") in vec2 texCoord;\n";
    vs << "uniform mat4 MVP;\n"
          "out vec3 pos;\n";
    if(norm2col)
        vs << "out vec3 nrm;\n";
    if(lightsEnabled)
    {
        vs << "uniform mat4 M;\n"
              "out vec3 fpos;\n";
        if(norm)
            vs << "uniform mat3 N;\n"
                  "out vec3 norm;\n";
    }
    if(tex)
        vs << "out vec2 uv;\n";
    vs << "void main() {\n"
          "    pos = vertexPos;\n"
          "    gl_Position = MVP * vec4(vertexPos, 1);\n";
    if(norm2col)
        vs << "    nrm = normal;\n";
    if(lightsEnabled)
    {
        vs << "    fpos = vec3(M * vec4(vertexPos,1));\n";
        if(norm)
            vs << "    norm = N * normal;\n";
    }
    if(tex)
        vs << "    uv = texCoord;\n";
    vs << "}";
// ----------fragement shader----------
    fs << "#version " << AGL_GLVERSION_MAJOR << AGL_GLVERSION_MINOR << "0 core\n"
          "in vec3 pos;\n";
    if(norm2col)
        fs << "in vec3 nrm;\n";
    if(tex)
        fs << "in vec2 uv;\n"
              "uniform sampler2D txtr;\n";
    if(lightsEnabled)
    {
       fs << (norm ? "in vec3 fpos, norm;\n" :  "in vec3 fpos;\n") <<
       "uniform vec4 emission, ambient, diffuse, specular;\n"
       "uniform float shininess;\n"
       "uniform vec3 vpos;\n\n"
       "#define NUM_LIGHTS " << lights.size() << "\n"
       "uniform struct Light {\n"
       "    vec4 ambient, diffuse, specular, position;\n"
       "    vec3 halfVector, spotDirection;\n"
       "    float spotExponent, spotCutoff, spotCosCutoff, constantAttenuation, linearAttenuation, quadraticAttenuation;\n"
       "} lights[NUM_LIGHTS];\n\n";
       if(lightingModel == AGL_LIGHTING_PBR)
       {
    fs << "float DGGX(vec3 N, vec3 H, float roughness)\n{\n"
          "    float a = roughness*roughness;\n"
          "    float a2 = a*a;\n"
          "    float NdotH = max(dot(N, H), 0.0);\n"
          "    float NdotH2 = NdotH*NdotH;\n"
          "    float nom   = a2;\n"
          "    float denom = (NdotH2 * (a2 - 1.0) + 1.0);\n"
          "    denom = " << AGL_PI << " * denom * denom;\n"
          "    return nom / max(denom, 0.001);\n"
          "}\n"
          "float GSGGX(float NdotV, float roughness)\n{\n"
          "    float r = (roughness + 1.0);\n"
          "    float k = (r*r) / 8.0;\n"
          "    float nom   = NdotV;\n"
          "    float denom = NdotV * (1.0 - k) + k;\n"
          "    return nom / denom;\n"
          "}\n"
          "float GS(vec3 N, vec3 V, vec3 L, float roughness)\n{\n"
          "    float NdotV = max(dot(N, V), 0.0);\n"
          "    float NdotL = max(dot(N, L), 0.0);\n"
          "    float ggx2 = GSGGX(NdotV, roughness);\n"
          "    float ggx1 = GSGGX(NdotL, roughness);\n"
          "    return ggx1 * ggx2;\n"
          "}\n"
          "vec4 FS(float cosTheta, vec4 F0)\n{\n"
          "    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);\n"
          "}\n\n";
       }
    }
    else if(emission.w >= 0)
        fs << "uniform vec4 emission;\n";
    fs << "out vec4 color;\n"
          "void main() {\n";
    if(lightsEnabled && lightingModel==AGL_LIGHTING_PBR)
    {// Albedo is in emission and metallic, roughness, ao and f0 is in specular
fs << "    vec3 N = normalize(" << (norm ? "norm" : "cross(dFdx(fpos), dFdy(fpos))") << "),\n"
      "         V = normalize(vpos - fpos), lightDir;\n";
        setupColors(this, e, fs, true, tex);  // F0 >= 0
fs << "    vec4 F0 = mix(vec4(specular.www, 1), emission, specular.x),\n"
      "         result = vec4(0);\n"
      "    for(int i=0; i<NUM_LIGHTS; ++i)\n    {\n"
      "        vec3 L = normalize(lights[i].position.xyz - fpos), H = normalize(V+L);\n"
      "        float d = distance(lights[i].position.xyz, fpos),\n"
      "              attenuation = lights[i].constantAttenuation + d * (lights[i].linearAttenuation +  d * lights[i].quadraticAttenuation);\n"
      "        vec4 radiance = lights[i].specular * attenuation;\n"
      "        float NDF = DGGX(N, H, specular.y), G = GS(N, V, L, specular.y);\n"
      "        vec4 F = FS(clamp(dot(H,V), 0, 1), F0), nom = NDF*G*F;\n"
      "        float denom = 4 * max(dot(N,V), 0) * max(dot(N,L), 0);\n"
      "        vec4 spec = nom / max(denom, 0.001), kD = 1-F;\n"
      "        kD *= 1-specular.x;\n"
      "        float NdotL = max(dot(N,L), 0);\n"
      "        result += (kD * emission / " << AGL_PI << " + spec) * radiance * NdotL + \n"
      "                  lights[i].ambient * emission * specular.z;\n"
      "    }\n"
      "    color = result;\n";
    }
    else if(lightsEnabled)
    {
 fs << "    vec3 norm = normalize(" << (norm ? "norm" : "cross(dFdx(fpos), dFdy(fpos))") << "),\n"
       "         viewDir = normalize(vpos - fpos), lightDir;\n"
       "    vec4 result = vec4(0);\n";
        setupColors(this, e, fs, true, tex);
 fs << "    for(int i=0; i<NUM_LIGHTS; ++i)\n    {\n"
       "        lightDir = normalize(lights[i].position.w==0 ? -lights[i].position.xyz : (lights[i].position.xyz - fpos));\n"
       "        float spotlight = 1, d;\n"
       "        if(lights[i].spotCosCutoff >= 0)\n        {\n"
       "            float spotCosine = dot(lightDir, -normalize(lights[i].spotDirection));\n"
       "            spotlight = spotCosine >= lights[i].spotCosCutoff ? pow(spotCosine, lights[i].spotExponent) : 0;\n"
       "        }\n"
       "        if(lights[i].position.w == 1)\n        {\n"
       "            d = distance(lights[i].position.xyz, fpos);\n"
       "            spotlight /= lights[i].constantAttenuation + d * (lights[i].linearAttenuation +  d * lights[i].quadraticAttenuation);\n"
       "        }\n"
       "        result += spotlight *\n"
       "                 (lights[i].ambient * ambient +\n"
       "                  max(dot(norm, lightDir), 0) * lights[i].diffuse * diffuse +\n"
       "                  pow(max(dot(";
        if(lightingModel == AGL_LIGHTING_PHONG)
            fs << "viewDir, reflect(-lightDir, norm";
        else if(lightingModel == AGL_LIGHTING_BLINNPHONG)
            fs << "norm, normalize(lightDir + viewDir";
                                fs << ")), 0), shininess) * lights[i].specular * specular);\n"
       "    }\n"
       "    color = emission + result;\n";
    }
    else  // ----------no lights----------
    {
        setupColors(this, e, fs, false, tex);
        fs << "    color = emission;\n";
    }
    fs << "}";
    std::cout << vs.str() << std::endl << fs.str() <<std::endl;
    return std::pair<std::string, std::string>(vs.str(), fs.str());
}
}
