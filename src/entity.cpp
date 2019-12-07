#include "entity.h"
#include "scene.h"
#include "util.h"
#include<sstream>
#include<glm/gtc/matrix_transform.hpp>

namespace agl {
BaseEntity::~BaseEntity() = default;

Entity::Entity(const glm::vec3 &pos): pos(pos){}
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
    transform(glm::scale(glm::mat4(1), glm::vec3(sx, sy, sz)));
}
void Entity::transform(const glm::mat4 &m)
{
    model = m * model;
}
void Entity::add(BaseEntity &e)
{
    children.push_back(&e);
    e.parent = this;
}
void Entity::createBuffers()
{
    glDeleteVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glDeleteBuffers(1, &VBO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glDeleteBuffers(1, &EBO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}
glm::mat4 Entity::getMatM()
{
    if(parent == nullptr)
        return model;
    else
        return parent->getMatM() * model;
}

Material::~Material()
{
    delete texture;
    glDeleteProgram(progID);
}
std::pair<std::string, std::string> Material::createShader(Entity *e)
{
    std::stringstream vs, fs;
    vs << "#version " << AGL_GLVERSION_MAJOR << AGL_GLVERSION_MINOR << "0 core\n"
          "layout(location = 0) in vec3 vertexPos;\n"
          "uniform mat4 MVP;\n";
    if(lightsEnabled)
        vs << "uniform mat4 N;\n";
    vs << "out vec3 pos;\n";
    if(lightsEnabled)
        vs << "out vec3 norm;\n";
    vs << "void main()\n{\n"
          "    gl_Position = MVP * vec4(vertexPos, 1);\n"
          "    pos = vertexPos;\n";
    if(lightsEnabled)
        vs << "    norm = N * vertexPos;\n";
    vs << "}";

    fs << "#version " << AGL_GLVERSION_MAJOR << AGL_GLVERSION_MINOR << "0 core\n"
          "in vec3 pos;\n";
    if(lightsEnabled)
        fs << "in vec3 norm;\n";
    if(ambient.w >= 0)
        fs << "uniform vec4 ambient;\n";
    fs << "out vec4 color;\n";
    fs << "void main()\n{\n";
    if(lightsEnabled);
    else if(ambient.w >= 0)
        fs << "    color = ambient;\n";
    else if(e != nullptr)
    {
        int l = e->vertices.size() / 3;
        float r1 = e->vertices[0], r2 = e->vertices[0],
              g1 = e->vertices[1], g2 = e->vertices[1],
              b1 = e->vertices[2], b2 = e->vertices[2];
        switch((int)ambient.w)
        {
        case AGL_COLOR_POS2RGB_NORMED:
            fs << "    color = vec4(";
            if(ambient.r == -1)
            {
                for(int i=0; i<l; ++i)
                {
                    r1 = std::min(r1, e->vertices[i*3]);
                    r2 = std::max(r2, e->vertices[i*3]);
                }
                fs << "(pos.r - " << r1 << ") / " << r2 - r1 << ", ";
            }
            else
                fs << ambient.r << ", ";
            if(ambient.g == -1)
            {
                for(int i=0; i<l; ++i)
                {
                    g1 = std::min(g1, e->vertices[i*3+1]);
                    g2 = std::max(g2, e->vertices[i*3+1]);
                }
                fs << "(pos.g - " << g1 << ") / " << g2 - g1 << ", ";
            }
            else
                fs << ambient.g << ", ";
            if(ambient.b == -1)
            {
                for(int i=0; i<l; ++i)
                {
                    b1 = std::min(b1, e->vertices[i*3+2]);
                    b2 = std::max(b2, e->vertices[i*3+2]);
                }
                fs << "(pos.b - " << b1 << ") / " << b2 - b1 << ", ";
            }
            else
                fs << ambient.b << ", ";
            fs << "1);\n";
            break;
        }
    }
    else
        fs << "    color = vec4(0.9, 0.9, 0.9, 1);\n";
    fs << "}";
    return std::pair<std::string, std::string>(vs.str(), fs.str());
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
            nID   = glGetUniformLocation(progID, "N");
            eID   = glGetUniformLocation(progID, "emission");
            dID   = glGetUniformLocation(progID, "diffuse");
            sID   = glGetUniformLocation(progID, "specular");
        }
    }
}
}
