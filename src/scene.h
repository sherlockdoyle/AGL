#ifndef SCENE_H
#define SCENE_H

#include "entity.h"
#include<vector>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>

namespace agl {
class Camera
{
public:
    glm::mat4 view;
    glm::vec3 _pos, _lookAt, _up;  // cache values

    Camera(const glm::vec3 &pos=glm::vec3(3, 4, 5), const glm::vec3 &lookAt=glm::vec3(0), const glm::vec3 &up=glm::vec3(0, 1, 0));
    void setPos(const glm::vec3 &pos=glm::vec3(3, 4, 5));
    void setPos(float x=3, float y=4, float z=5);
    void lookAt(const glm::vec3 &target=glm::vec3(0));
    void lookAt(float x=0, float y=0, float z=0);
    void setUp(const glm::vec3 &up=glm::vec3(0, 1, 0));
    void setUp(float x=0, float y=1, float z=0);
    void setView(const glm::mat4 &mat);
    void rotate(float rad=0, const glm::vec3 &axis=glm::vec3(0), bool around=true);
    void rotate(const glm::quat &rot=glm::quat(1, 0, 0, 0), bool around=true);
    void rotateX(float rad=0, bool around=true, bool safe=true);
    void rotateY(float rad=0, bool around=true);
    void rotateZ(float rad=0, bool around=true);
    void translate(const glm::vec3 &d=glm::vec3(0));
    void translate(float dx=0, float dy=0, float dz=0);
    void viewTranslate(const glm::vec3 &d=glm::vec3(0));
    void viewTranslate(float dx=0, float dy=0, float dz=0);
    void viewRotate(float rad=0, const glm::vec3 &axis=glm::vec3(0));
    void viewScale(const glm::vec3 &s=glm::vec3(1));
    void viewScale(float sx=1, float sy=1, float sz=1);
    void viewTransform(const glm::mat4 &m);
};

class Scene
{
public:
    GLFWwindow* window;
    Camera camera;
    std::vector<BaseEntity*> children;

    Scene(int width=640, int height=480, const char *name="AGL");
    ~Scene();
    void setPerspectiveProjection(float fovDeg=45, float aspectRatio=-1, float near=0.1, float far=100);
    void setOrthographicProjection(float left=-1, float right=1, float bottom=-1, float top=1);
    void setCamera(const glm::vec3 &pos, const glm::vec3 &lookAt, const glm::vec3 &up);
    void setProjection(glm::mat4 mat);
    void setView(glm::mat4 mat);
    void setController(GLFWkeyfun keyCallback=nullptr, GLFWcursorposfun cursorPosCallback=nullptr,
                       GLFWmousebuttonfun mouseButtonCallback=nullptr, GLFWscrollfun scrollCallback=nullptr,
                       GLFWwindowsizefun windowSizeCallback=nullptr);
    void setBGcolor(float r=0, float g=0, float b=0, float a=0);
    void add(BaseEntity &e);
    void prepare();
    void render();
    glm::mat4 getMatVP();

private:
    int width, height;
    glm::vec4 bgcolor;
    glm::mat4 projection;
    std::vector<Entity*> entities;
    std::vector<Light*> lights;

    int setupGL(const char *name);
    void getAllEntity(std::vector<BaseEntity*> &children);
};

void defKeyCB(GLFWwindow* window, int key, int scancode, int action, int mods);
void defScrollCB(GLFWwindow* window, double xoffset, double yoffset);
}

#endif // SCENE_H
