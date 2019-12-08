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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

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
            std::pair<char, char> coord;
            if(ambient.x == -1)
            {
                coord.first = 'x';
                coord.second = ambient.y == -1 ? 'y' : 'z';
            }
            else
            {
                coord.first = 'y';
                coord.second = 'z';
            }
            fs << "    float f = mod(floor(pos." << coord.first << ") + floor(pos." << coord.second << "), 2);\n"
                  "    color = vec4(f, f, f, 1);\n";
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
