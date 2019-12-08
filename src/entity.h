#ifndef ENTITY_H
#define ENTITY_H

#include<vector>
#include<GL/gl.h>
#include<GLES3/gl32.h>
#include<glm/glm.hpp>
#include "util.h"

namespace agl {
class Entity;
class Light;
class Scene;

class Material
{
public:
    glm::vec4 emission,    // Ecm
              ambient,     // Acm
              diffuse,     // Dcm
              specular;    // Scm
    float shininess = 0;   // Srm
    bool lightsEnabled = false, customShader = false;
    int shadingType = AGL_SHADING_PHONG,
        tex_width = -1, tex_height = -1, tex_channel = -1;
    GLubyte *texture = nullptr;
    GLuint progID = 0, mvpID, nID, eID, aID, dID, sID;
    ~Material();
    std::pair<std::string, std::string> createShader(Entity *e=nullptr);
    void setShader(std::string vertexShader, std::string fragmentShader);
};

class BaseEntity
{
public:
    Entity *parent = nullptr;
    virtual ~BaseEntity();
};

class Entity: virtual public BaseEntity
{
public:
    GLuint VAO = 0, VBO = 0, EBO = 0;
//           polyMode = GL_FILL;
    bool dynamic = false;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    glm::vec3 pos;
    glm::mat4 model;
    Material material;
    std::vector<BaseEntity*> children;
    Entity(const glm::vec3 &pos=glm::vec3(0));
    ~Entity();
    void translate(const glm::vec3 &d=glm::vec3(0));
    void translate(float dx=0, float dy=0, float dz=0);
    void rotate(float rad=0, glm::vec3 axis=glm::vec3(0));
    void scale(const glm::vec3 &s=glm::vec3(1));
    void scale(float sx=1, float sy=1, float sz=1);
    void transform(const glm::mat4 &m);
    void add(BaseEntity &e);
    void createBuffers();
    glm::mat4 getMatM();
};

class Light: virtual public BaseEntity
{
public:
    glm::vec4 ambient,         // Aclarri
              diffuse,         // Dcli
              specular,        // Scli
              position,        // Ppli
              halfVector,      // Derived: Hi
              spotDirection;   // Sdli
    float spotExponent,        // Srli
          spotCutoff,          // Crli (range: [0.0,90.0], 180.0)
          spotCosCutoff,       // Derived: cos(Crli) (range: [1.0,0.0],-1.0)
          constantAttenuation, // K0
          linearAttenuation,   // K1
          quadraticAttenuation;// K2
};
}

#endif // ENTITY_H
