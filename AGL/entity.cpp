#include "entity.h"
#include "scene.h"
#include "util.h"
#include "glm/gtc/matrix_transform.hpp"

namespace agl {
BaseEntity::~BaseEntity() = default;

Entity::Entity(const glm::vec3 &pos): position(pos){}
Entity::~Entity()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void Entity::translate(const glm::vec3 &d)
{
    transform(glm::translate(glm::mat4(1), d));
}
void Entity::translate(float dx, float dy, float dz)
{
    transform(glm::translate(glm::mat4(1), glm::vec3(dx, dy, dz)));
}
void Entity::rotate(float rad, glm::vec3 axis)
{
    transform(glm::rotate(glm::mat4(1), rad, axis));
}
void Entity::scale(const glm::vec3 &s)
{
    transform(glm::scale(glm::mat4(1), s));
}
void Entity::scale(float sx, float sy, float sz)
{
    if(std::isnan(sy))
        sz = sy = sx;
    else if(std::isnan(sz))
        sz = 1;
    transform(glm::scale(glm::mat4(1), glm::vec3(sx, sy, sz)));
}
void Entity::transform(const glm::mat4 &m)
{
    model = m * model;
}
void Entity::applyTransform()
{
    glm::vec4 transformed;
    for(int i=0, l=vertices.size(); i<l; i+=3)
    {
        transformed = model * glm::vec4(vertices[i], vertices[i+1], vertices[i+2], 1);
        vertices[i  ] = transformed.x;
        vertices[i+1] = transformed.y;
        vertices[i+2] = transformed.z;
    }
    model = glm::mat4();
}
void Entity::add(BaseEntity &e)
{
    children.push_back(&e);
    e.parent = this;
}
void Entity::mergeData()
{
    bool norm = !normals.empty();
    merged.clear();
    for(int i=0, l=vertices.size(); i<l; i+=3)
    {
        merged.push_back(vertices[i  ]);
        merged.push_back(vertices[i+1]);
        merged.push_back(vertices[i+2]);
        if(norm)
        {
            merged.push_back(normals[i  ]);
            merged.push_back(normals[i+1]);
            merged.push_back(normals[i+2]);
        }
    }
}
void Entity::createBuffers()
{
    glDeleteVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glDeleteBuffers(1, &VBO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, merged.size() * sizeof(GLfloat), &merged[0], dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    bool norm = !normals.empty(), uv = false;
    int stride = norm ? uv ? 8 : 6 : uv ? 5 : 3;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    if(norm)
    {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }
    if(uv)
    {
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)((norm ? 6 : 3) * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    }

    glDeleteBuffers(1, &EBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}
glm::mat4 Entity::getMatM()
{
    if(parent == nullptr)
        return glm::translate(glm::mat4(1), position) * model;
    else
        return glm::translate(glm::mat4(1), position) * (parent->getMatM() * model);
}

Material::Material() = default;
Material::Material(float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, float sn):
    ambient(ar, ag, ab, 1), diffuse(dr, dg, db, 1), specular(sr, sg, sb, 1), shininess(sn) {}
Material::~Material()
{
    glDeleteProgram(progID);
    delete texture;
}
void Material::setColor(glm::vec4 color)
{
    emission = ambient = diffuse = specular = color;
}
void Material::setColor(float r, float g, float b, float a)
{
    emission = ambient = diffuse = specular = glm::vec4(r, g, b, a);
}
void Material::setShader(std::string vertexShader, std::string fragmentShader)
{
    progID = loadShaders(vertexShader.c_str(), fragmentShader.c_str());
    if(!customShader)
    {
        mvpID = glGetUniformLocation(progID, "MVP");
        aID   = glGetUniformLocation(progID, "ambient");
        if(lightsEnabled)  // only calculate if lights enabled
        {
            mID = glGetUniformLocation(progID, "M");
            nID = glGetUniformLocation(progID, "N");
            eID = glGetUniformLocation(progID, "emission");
            dID = glGetUniformLocation(progID, "diffuse");
            sID = glGetUniformLocation(progID, "specular");
            gID = glGetUniformLocation(progID, "shininess");
            vID = glGetUniformLocation(progID, "vpos");
        }
    }
}

Light::Light(const glm::vec3 &pos, glm::vec4 color): ambient(color), diffuse(color), specular(color), position(pos) {}
Light::Light(const glm::vec3 &pos, float r, float g, float b, float a): ambient(r, g, b, a), diffuse(r, g, b, a),
    specular(r, g, b, a), position(pos) {}
void Light::setColor(glm::vec4 color)
{
    ambient = diffuse = specular = color;
}
void Light::setColor(float r, float g, float b, float a)
{
    ambient = diffuse = specular = glm::vec4(r, g, b, a);
}

const Material Material::emerald = Material(0.0215, 0.1745, 0.0215, 0.07568, 0.61424, 0.07568, 0.633, 0.727811, 0.633, 76.8);
const Material Material::jade = Material(0.135, 0.2225, 0.1575, 0.54, 0.89, 0.63, 0.316228, 0.316228, 0.316228, 12.8);
const Material Material::obsidian = Material(0.05375, 0.05, 0.06625, 0.18275, 0.17, 0.22525, 0.332741, 0.328634, 0.346435, 38.4);
const Material Material::pearl = Material(0.25, 0.20725, 0.20725, 1, 0.829, 0.829, 0.296648, 0.296648, 0.296648, 11.264);
const Material Material::ruby = Material(0.1745, 0.01175, 0.01175, 0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 0.626959, 76.8);
const Material Material::turquoise = Material(0.1, 0.18725, 0.1745, 0.396, 0.74151, 0.69102, 0.297254, 0.30829, 0.306678, 12.8);
const Material Material::brass = Material(0.329412, 0.223529, 0.027451, 0.780392, 0.568627, 0.113725, 0.992157, 0.941176, 0.807843, 27.89743616);
const Material Material::bronze = Material(0.2125, 0.1275, 0.054, 0.714, 0.4284, 0.18144, 0.393548, 0.271906, 0.166721, 25.6);
const Material Material::chrome = Material(0.25, 0.25, 0.25, 0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 76.8);
const Material Material::copper = Material(0.19125, 0.0735, 0.0225, 0.7038, 0.27048, 0.0828, 0.256777, 0.137622, 0.086014, 12.8);
const Material Material::gold = Material(0.24725, 0.1995, 0.0745, 0.75164, 0.60648, 0.22648, 0.628281, 0.555802, 0.366065, 51.2);
const Material Material::silver = Material(0.19225, 0.19225, 0.19225, 0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 51.2);
const Material Material::black_plastic = Material(0, 0, 0, 0.01, 0.01, 0.01, 0.50, 0.50, 0.50, 32);
const Material Material::cyan_plastic = Material(0, 0.1, 0.06, 0, 0.50980392, 0.50980392, 0.50196078, 0.50196078, 0.50196078, 32);
const Material Material::green_plastic = Material(0, 0, 0, 0.1, 0.35, 0.1, 0.45, 0.55, 0.45, 32);
const Material Material::red_plastic = Material(0, 0, 0, 0.5, 0, 0, 0.7, 0.6, 0.6, 32);
const Material Material::white_plastic = Material(0, 0, 0, 0.55, 0.55, 0.55, 0.70, 0.70, 0.70, 32);
const Material Material::yellow_plastic = Material(0, 0, 0, 0.5, 0.5, 0, 0.60, 0.60, 0.50, 32);
const Material Material::black_rubber = Material(0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.4, 0.4, 0.4, 10);
const Material Material::cyan_rubber = Material(0, 0.05, 0.05, 0.4, 0.5, 0.5, 0.04, 0.7, 0.7, 10);
const Material Material::green_rubber = Material(0, 0.05, 0, 0.4, 0.5, 0.4, 0.04, 0.7, 0.04, 10);
const Material Material::red_rubber = Material(0.05, 0, 0, 0.5, 0.4, 0.4, 0.7, 0.04, 0.04, 10);
const Material Material::white_rubber = Material(0.05, 0.05, 0.05, 0.5, 0.5, 0.5, 0.7, 0.7, 0.7, 10);
const Material Material::yellow_rubber = Material(0.05, 0.05, 0, 0.5, 0.5, 0.4, 0.7, 0.7, 0.04, 10);
}
