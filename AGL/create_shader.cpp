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
}
std::pair<std::string, std::string> Material::createShader(Entity *e, std::vector<Light*> lights)
{
    std::stringstream vs, fs;
    bool norm = !e->normals.empty();
// ----------vertex shader----------
    vs << "#version " << AGL_GLVERSION_MAJOR << AGL_GLVERSION_MINOR << "0 core\n"
          "layout(location = 0) in vec3 vertexPos;\n";
    if(norm)
        vs << "layout(location = 1) in vec3 normal;\n";
    vs << "uniform mat4 MVP;\n"
          "out vec3 pos;\n";
    if(lightsEnabled)
    {
        vs << "uniform mat4 M;\n"
              "out vec3 fpos;\n";
        if(norm)
            vs << "uniform mat3 N;\n"
                  "out vec3 norm;\n";
    }
    vs << "void main() {\n"
          "    pos = vertexPos;\n"
          "    gl_Position = MVP * vec4(vertexPos, 1);\n";
    if(lightsEnabled)
    {
        vs << "    fpos = vec3(M * vec4(vertexPos,1));\n";
        if(norm)
            vs << "    norm = N * normal;\n";
    }
    vs << "}";
// ----------fragement shader----------
    fs << "#version " << AGL_GLVERSION_MAJOR << AGL_GLVERSION_MINOR << "0 core\n"
          "in vec3 pos;\n";
    if(lightsEnabled)
    {
       fs << (norm ? "in vec3 fpos, norm;\n" :  "in vec3 fpos;\n") <<
       "uniform vec4 emission, ambient, diffuse, specular;\n"
       "uniform float shininess;\n"
       "uniform vec3 vpos;\n\n"
       "uniform struct Light {\n"
       "    vec4 ambient, diffuse, specular, position;\n"
       "    vec3 halfVector, spotDirection;\n"
       "    float spotExponent, spotCutoff, spotCosCutoff, constantAttenuation, linearAttenuation, quadraticAttenuation;\n"
       "} lights[" << lights.size() << "];\n\n";
    }
    else if(ambient.w >= 0)
        fs << "uniform vec4 ambient;\n";
    fs << "out vec4 color;\n"
          "void main() {\n";
    if(lightsEnabled)
    {
 fs << "    vec3 norm = normalize(" << (norm ? "norm" : "cross(dFdx(fpos), dFdy(fpos))") << "), viewDir = normalize(vpos - fpos), lightDir;\n"
       "    vec4 result = vec4(0);\n";
 fs << "    for(int i=0; i<" << lights.size() << "; ++i)\n    {\n"
       "        lightDir = normalize(lights[i].position.w==0 ? -lights[i].position.xyz : (lights[i].position.xyz - fpos));\n"
       "        float spotlight = 1, d, attenuation;\n"
       "        if(lights[i].spotCosCutoff >= 0)\n        {\n"
       "            float spotCosine = dot(lightDir, -normalize(lights[i].spotDirection));\n"
       "            spotlight = spotCosine >= lights[i].spotCosCutoff ? pow(spotCosine, lights[i].spotExponent) : 0;\n"
       "        }\n"
       "        if(lights[i].position.w == 1)\n        {\n"
       "            d = distance(lights[i].position.xyz, fpos);\n"
       "            spotlight /= lights[i].constantAttenuation + d * (lights[i].linearAttenuation +  d * lights[i].quadraticAttenuation);\n"
       "        }\n"
       "        result += emission + spotlight *\n"
       "                 (lights[i].ambient * ambient +\n"
       "                  max(dot(norm, lightDir), 0) * lights[i].diffuse * diffuse +\n"
       "                  pow(max(dot(viewDir, reflect(-lightDir, norm)), 0), shininess) * lights[i].specular * specular);\n"
       "    }\n"
       "    color = result;\n";
    }
    else if(ambient.w >= 0)  // ----------no lights----------
        fs << "    color = ambient;\n";
    else if(e != nullptr)
    {
        int l = e->vertices.size() / 3;
        std::pair<GLfloat, GLfloat> mnmx;
        switch((int)ambient.w)
        {
        case AGL_COLOR_POS2RGB_NORMED:
            fs << "    color = vec4(";
            if(ambient.r == -1)
            {
                mnmx = getMinMax(e->vertices, l, 3, 0);
                fs << "(pos.r - " << mnmx.first << ") / " << mnmx.second - mnmx.first << ", ";
            }
            else
                fs << ambient.r << ", ";
            if(ambient.g == -1)
            {
                mnmx = getMinMax(e->vertices, l, 3, 1);
                fs << "(pos.g - " << mnmx.first << ") / " << mnmx.second - mnmx.first << ", ";
            }
            else
                fs << ambient.g << ", ";
            if(ambient.b == -1)
            {
                mnmx = getMinMax(e->vertices, l, 3, 2);
                fs << "(pos.b - " << mnmx.first << ") / " << mnmx.second - mnmx.first << ", ";
            }
            else
                fs << ambient.b << ", ";
            fs << "1);\n";
            break;
        case AGL_COLOR_CHECKERBOARD:
            fs << "    float f = mod(";
            if(ambient.x == -1)
                fs << "+floor(pos.x)";
            if(ambient.y == -1)
                fs << "+floor(pos.y)";
            if(ambient.z == -1)
                fs << "+floor(pos.z)";
            fs << ", 2);\n"
                  "    color = vec4(f, f, f, 1);\n";
            break;
        }
    }
    else
        fs << "    color = vec4(0.9, 0.95, 1, 1);\n";
    fs << "}";
//    std::cout << vs.str() << std::endl << fs.str() <<std::endl;
    return std::pair<std::string, std::string>(vs.str(), fs.str());
}
}
