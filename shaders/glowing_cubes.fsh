#version 450 core
in vec3 pos;
in vec2 uv;
uniform sampler2D txtr;
in vec3 fpos, norm;
uniform vec4 emission, ambient, diffuse, specular;
uniform float shininess;
uniform vec3 vpos;

uniform struct Light {
    vec4 ambient, diffuse, specular, position;
    vec3 halfVector, spotDirection;
    float spotExponent, spotCutoff, spotCosCutoff, constantAttenuation, linearAttenuation, quadraticAttenuation;
} lights[1];

out vec4 color;
void main() {
    vec3 norm = normalize(cross(dFdx(fpos), dFdy(fpos))),
         viewDir = normalize(vpos - fpos), lightDir;
    vec4 result = vec4(0);
    vec4 ambient = texture(txtr, uv),
         diffuse = ambient, specular = ambient;
    vec4 emission = (1-clamp(ambient, vec4(0), vec4(.5))/.5) * emission;
    for(int i=0; i<1; ++i)
    {
        lightDir = normalize(lights[i].position.w==0 ? -lights[i].position.xyz : (lights[i].position.xyz - fpos));
        float spotlight = 1, d, attenuation;
        if(lights[i].spotCosCutoff >= 0)
        {
            float spotCosine = dot(lightDir, -normalize(lights[i].spotDirection));
            spotlight = spotCosine >= lights[i].spotCosCutoff ? pow(spotCosine, lights[i].spotExponent) : 0;
        }
        if(lights[i].position.w == 1)
        {
            d = distance(lights[i].position.xyz, fpos);
            spotlight /= lights[i].constantAttenuation + d * (lights[i].linearAttenuation +  d * lights[i].quadraticAttenuation);
        }
        result += emission + spotlight *
                 (lights[i].ambient * ambient +
                  max(dot(norm, lightDir), 0) * lights[i].diffuse * diffuse +
                  pow(max(dot(viewDir, reflect(-lightDir, norm)), 0), shininess) * lights[i].specular * specular);
    }
    color = result;
}
