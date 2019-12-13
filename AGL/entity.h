#ifndef ENTITY_H
#define ENTITY_H

#include<vector>
#include<GL/gl.h>
#include<GLES3/gl32.h>
#include "glm/glm.hpp"
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
    float shininess = 32;   // Srm
    bool lightsEnabled = false, customShader = false;
    int shadingType = AGL_SHADING_PHONG,
        tex_width = -1, tex_height = -1, tex_channel = -1;
    GLubyte *texture = nullptr;
    GLuint progID = 0, mvpID, mID, nID, eID, aID, dID, sID, gID, vID;

    Material();
    ~Material();
    Material operator+(const Material &other) const;
    Material &operator+=(const Material &other);
    Material operator*(const Material &other) const;
    Material &operator*=(const Material &other);
    void setColor(glm::vec4 color=glm::vec4(1));
    void setColor(float r=1, float g=1, float b=1, float a=1);
    void setColor(const Material &other);
    virtual std::pair<std::string, std::string> createShader(Entity *e=nullptr, std::vector<Light*> lights=std::vector<Light*>());
    virtual void setShader(std::string vertexShader, std::string fragmentShader, bool forceSet=false);

    static const Material emerald, jade, obsidian, pearl, ruby, turquoise, brass, bronze, chrome, copper, gold, silver,
                          black_plastic, cyan_plastic, green_plastic, red_plastic, white_plastic, yellow_plastic,
                          black_rubber,  cyan_rubber,  green_rubber,  red_rubber,  white_rubber,  yellow_rubber;
private:
    Material(float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, float sn);
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
    std::vector<GLfloat> vertices, normals,
        merged;
    std::vector<GLuint> indices;
    glm::vec3 position;
    glm::mat4 model;
    Material material;
    std::vector<BaseEntity*> children;

    Entity(const glm::vec3 &pos=glm::vec3(0));
    Entity(const Entity &other);
    ~Entity();
    void translate(const glm::vec3 &d=glm::vec3(0));
    void translate(float dx=0, float dy=0, float dz=0);
    void rotate(float rad=0, glm::vec3 axis=glm::vec3(0));
    void scale(const glm::vec3 &s=glm::vec3(1));
    void scale(float sx=1, float sy=std::nanf(""), float sz=std::nanf(""));
    void transform(const glm::mat4 &m);
    void applyTransform();
    void add(BaseEntity &e);
    virtual void mergeData();
    virtual void createBuffers();
    glm::mat4 getMatM();
};

class Light: virtual public BaseEntity
{
public:
    glm::vec4 ambient,         // Aclarri
              diffuse,         // Dcli
              specular,        // Scli
              position;        // Ppli
    glm::vec3 spotDirection;   // Sdli
    float spotExponent  =  0,  // Srli
          spotCosCutoff = -1,  // Derived: cos(Crli) (range: [1.0,0.0],-1.0)
          constantAttenuation  = 1, // K0
          linearAttenuation    = 0, // K1
          quadraticAttenuation = 0; // K2

    Light(const glm::vec3 &pos=glm::vec3(0), const glm::vec4 &color=glm::vec4(1));
    Light(const glm::vec3 &pos=glm::vec3(0), float r=1, float g=1, float b=1, float a=1);
    void setColor(glm::vec4 color=glm::vec4(1));
    void setColor(float r=1, float g=1, float b=1, float a=1);
    inline glm::vec4 getPos()
    {
        return parent == nullptr ? position : parent->getMatM() * position;
    }
};
inline Light DirectionalLight(const glm::vec3 &dir=glm::vec3(0), const glm::vec4 &color=glm::vec4(1))
{
    Light l(dir, color);
    l.position.w = 0;
    return l;
}
inline Light DirectionalLight(const glm::vec3 &dir=glm::vec3(0), float r=1, float g=1, float b=1, float a=1)
{
    Light l(dir, r, g, b, a);
    l.position.w = 0;
    return l;
}

class SharedEntity: virtual public Entity
{
public:
    Entity *source = nullptr;

    SharedEntity(Entity &src);
    virtual void mergeData();
    virtual void createBuffers();
};
}

#endif // ENTITY_H
