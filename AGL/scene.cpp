#include "scene.h"
#include "util.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/projection.hpp"

namespace agl {
Scene::Scene(int width, int height, const char *name)
{
    this->width = width;
    this->height = height;
    projection = glm::perspective(glm::radians(45.f), float(width)/height, 0.1f, 100.f);

    switch(setupGL(name))
    {
    case AGL_GLFW_INIT_ERROR:
        printf("Could not initialize GLFW.");
        break;
    case AGL_GLFW_CREATE_WINDOW_ERROR:
        printf("Could not create window.");
        break;
    }
    setController(defKeyCB, nullptr, nullptr, defScrollCB, nullptr);
}
Scene::~Scene()
{
    if(window)
        glfwDestroyWindow(window);
}
void Scene::setProjection(glm::mat4 mat)
{
    projection = mat;
}
void Scene::setPerspectiveProjection(float fovDeg, float aspectRatio, float near, float far)
{
    if(aspectRatio < 0)
        aspectRatio = float(width) / height;
    projection = glm::perspective(glm::radians(fovDeg), aspectRatio, near, far);
}
void Scene::setOrthographicProjection(float left, float right, float bottom, float top)
{
    projection = glm::ortho(left, right, bottom, top);
}
void Scene::setView(glm::mat4 mat)
{
    camera.setView(mat);
}
void Scene::setController(GLFWkeyfun keyCallback, GLFWcursorposfun cursorPosCallback,
                          GLFWmousebuttonfun mouseButtonCallback, GLFWscrollfun scrollCallback,
                          GLFWwindowsizefun windowSizeCallback)
{
    if(keyCallback)
        glfwSetKeyCallback(window, keyCallback);
    if(cursorPosCallback)
        glfwSetCursorPosCallback(window, cursorPosCallback);
    if(mouseButtonCallback)
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
    if(scrollCallback)
        glfwSetScrollCallback(window, scrollCallback);
    if(windowSizeCallback)
        glfwSetWindowSizeCallback(window, windowSizeCallback);
}
void Scene::setBGcolor(float r, float g, float b, float a)
{
    bgcolor = glm::vec4(r, g, b, a);
    glClearColor(bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
}
void Scene::add(BaseEntity &e)
{
    children.push_back(&e);
}
void Scene::prepare()
{
    glClearColor(bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.a);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    lights.clear();
    entities.clear();
    getAllEntity(children);
    for(Entity *e: entities)
    {
        std::pair<std::string, std::string> shaders = e->material.createShader(e);
        e->material.setShader(shaders.first, shaders.second);
        e->createBuffers();
    }
}
void Scene::render()
{
    glm::mat4 vp = getMatVP();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(Entity *e: entities)
    {
//        glPolygonMode(GL_FRONT_AND_BACK, e->polyMode);
        glm::mat4 mvp = vp * e->getMatM();
        glUseProgram(e->material.progID);
        glUniformMatrix4fv(e->material.mvpID, 1, GL_FALSE, &mvp[0][0]);
        glUniform4fv(e->material.aID, 1, &e->material.ambient[0]);
        if(e->material.lightsEnabled)
        {
            glUniform4fv(e->material.eID, 1, &e->material.emission[0]);
            glUniform4fv(e->material.dID, 1, &e->material.diffuse[0]);
            glUniform4fv(e->material.sID, 1, &e->material.specular[0]);
        }
        glBindVertexArray(e->VAO);
        if(e->dynamic)
        {
            glBindBuffer(GL_ARRAY_BUFFER, e->VBO);
            glBufferData(GL_ARRAY_BUFFER, e->vertices.size() * sizeof(GLfloat), &e->vertices[0], GL_DYNAMIC_DRAW);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, e->EBO);
        glDrawElements(GL_TRIANGLES, e->indices.size(), GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}
void Scene::setCamera(const glm::vec3 &pos, const glm::vec3 &lookAt, const glm::vec3 &up)
{
    camera = Camera(pos, lookAt, up);
}
glm::mat4 Scene::getMatVP()
{
    return projection * camera.view;
}
int Scene::setupGL(const char *name)
{
    if(!glfwInit())
        return AGL_GLFW_INIT_ERROR;
    glfwWindowHint(GLFW_SAMPLES, AGL_SAMPLE_SIZE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, AGL_GLVERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, AGL_GLVERSION_MINOR);
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if(!window)
    {
        glfwTerminate();
        return AGL_GLFW_CREATE_WINDOW_ERROR;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    return 0;
}
void Scene::getAllEntity(std::vector<BaseEntity*> &children)
{
    for(BaseEntity *e: children)
        if(Light *lght = dynamic_cast<Light*>(e))
            lights.push_back(lght);
        else
        {
            Entity *ent = dynamic_cast<Entity*>(e);
            entities.push_back(ent);
            if(ent->children.size() > 0)
               getAllEntity(ent->children);
        }
}

Camera::Camera(const glm::vec3 &pos, const glm::vec3 &lookAt, const glm::vec3 &up)
{
    _pos = pos;
    _lookAt = lookAt;
    _up = up;
    view = glm::lookAt(pos, lookAt, up);
}
void Camera::setPos(const glm::vec3 &pos)
{
    _pos = pos;
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::setPos(float x, float y, float z)
{
    _pos = glm::vec3(x, y, z);
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::lookAt(const glm::vec3 &target)
{
    _lookAt = target;
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::lookAt(float x, float y, float z)
{
    _lookAt = glm::vec3(x, y, z);
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::setUp(const glm::vec3 &up)
{
    _up = up;
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::setUp(float x, float y, float z)
{
    _up = glm::vec3(x, y, z);
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::setView(const glm::mat4 &mat)
{
    view = mat;
    // TODO: Implement extraction
    _pos = glm::vec3(std::nanf(""));
    _lookAt = glm::vec3(std::nanf(""));
    _up = glm::vec3(std::nanf(""));
}
void Camera::rotate(float rad, const glm::vec3 &axis, bool around)
{
    if(around)
        _pos = glm::rotate(_pos - _lookAt, rad, axis) + _lookAt;
    else
        _lookAt = glm::rotate(_lookAt - _pos, rad, axis) + _pos;
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::rotate(const glm::quat &rot, bool around)  // TODO: Is this code wrong?
{
    glm::vec3 q(rot.x, rot.y, rot.z);
    if(around)
    {
        _pos -= _lookAt;
        _pos = 2.f * (glm::dot(q, _pos) * q + rot.w * glm::cross(q, _pos)) + (rot.w * rot.w - glm::dot(q, q)) * _pos + _lookAt;
    }
    else
    {
        _lookAt -= _pos;
        _lookAt = 2.f * (glm::dot(q, _lookAt) * q + rot.w * glm::cross(q, _lookAt)) + (rot.w * rot.w - glm::dot(q, q)) * _lookAt + _pos;
    }
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::rotateX(float rad, bool around, bool safe)
{
    if(around)
        _pos = glm::rotateX(_pos - _lookAt, rad) + _lookAt;
    else
        _lookAt = glm::rotateX(_lookAt - _pos, rad) + _pos;
    if(safe)
        _up = glm::vec3(0, glm::sign(_pos.z), 0);
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::rotateY(float rad, bool around)
{
    if(around)
        _pos = glm::rotateY(_pos - _lookAt, rad) + _lookAt;
    else
        _lookAt = glm::rotateY(_lookAt - _pos, rad) + _pos;
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::rotateZ(float rad, bool around)
{
    if(around)
        _pos = glm::rotateZ(_pos - _lookAt, rad) + _lookAt;
    else
        _lookAt = glm::rotateZ(_lookAt - _pos, rad) + _pos;
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::translate(const glm::vec3 &d)
{
    _pos += d;
    _lookAt += d;
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::translate(float dx, float dy, float dz)
{
    _pos.x += dx; _pos.y += dy; _pos.z += dz;
    _lookAt.x += dx; _lookAt.y += dy; _lookAt.z += dz;
    view = glm::lookAt(_pos, _lookAt, _up);
}
void Camera::viewTranslate(const glm::vec3 &d)
{
    viewTransform(glm::translate(glm::mat4(1), d));
}
void Camera::viewTranslate(float dx, float dy, float dz)
{
    viewTransform(glm::translate(glm::mat4(1), glm::vec3(dx, dy, dz)));
}
void Camera::viewRotate(float rad, const glm::vec3 &axis)
{
    viewTransform(glm::rotate(glm::mat4(1), rad, axis));
}
void Camera::viewScale(const glm::vec3 &s)
{
    viewTransform(glm::translate(glm::mat4(1), s));
}
void Camera::viewScale(float sx, float sy, float sz)
{
    viewTransform(glm::scale(glm::mat4(1), glm::vec3(sx, sy, sz)));
}
void Camera::viewTransform(const glm::mat4 &m)
{
    view = m * view;
}

void defKeyCB(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // W: Camera forward
    // A: Camera left
    // S: Camera backward
    // D: Camera right
    // Q: Camera down
    // E: Camera up
    // 0: Reset camera
    Camera &camera = static_cast<Scene*>(glfwGetWindowUserPointer(window))->camera;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if(key == GLFW_KEY_0 || key == GLFW_KEY_KP_0)
        camera.view = glm::lookAt(camera._pos=glm::vec3(3, 4, 5), camera._lookAt=glm::vec3(0), camera._up=glm::vec3(0, 1, 0));
    if(key == GLFW_KEY_UP || key == GLFW_KEY_W)
    {
        glm::vec3 d = camera._pos - camera._lookAt;
        d.y = 0;
        d = glm::normalize(d) * 0.1f;
        camera.view = glm::lookAt(camera._pos -= d, camera._lookAt -= d, camera._up);
    }
    if(key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
    {
        glm::vec3 d = camera._pos - camera._lookAt;
        d.y = 0;
        d = glm::normalize(d) * 0.1f;
        camera.view = glm::lookAt(camera._pos += d, camera._lookAt += d, camera._up);
    }
    if(key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
    {
        glm::vec3 d = camera._pos - camera._lookAt;
        d.y = d.x; d.x = d.z; d.z = -d.y; d.y = 0;
        d = glm::normalize(d) * 0.1f;
        camera.view = glm::lookAt(camera._pos -= d, camera._lookAt -= d, camera._up);
    }
    if(key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
    {
        glm::vec3 d = camera._pos - camera._lookAt;
        d.y = d.x; d.x = d.z; d.z = -d.y; d.y = 0;
        d = glm::normalize(d) * 0.1f;
        camera.view = glm::lookAt(camera._pos += d, camera._lookAt += d, camera._up);
    }
    if(key == GLFW_KEY_Q)
    {
        camera._pos.y -= 0.1;
        camera._lookAt.y -= 0.1;
        camera.view = glm::lookAt(camera._pos, camera._lookAt, camera._up);
    }
    if(key == GLFW_KEY_E)
    {
        camera._pos.y += 0.1;
        camera._lookAt.y += 0.1;
        camera.view = glm::lookAt(camera._pos, camera._lookAt, camera._up);
    }
}
void defScrollCB(GLFWwindow *window, double xoffset, double yoffset)
{
    // Scroll: rotate camera
    Camera &camera = static_cast<Scene*>(glfwGetWindowUserPointer(window))->camera;
    camera.rotateY(-0.05 * xoffset, false);
    glm::vec3 d = camera._pos - camera._lookAt;
    d.y = d.x; d.x = d.z; d.z = -d.y; d.y = 0;
    camera.rotate(-0.05 * yoffset, glm::normalize(d), false);
}
}
