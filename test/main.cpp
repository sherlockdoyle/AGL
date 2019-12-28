#include<sstream>
#include "../AGL/agl.h"
#include "../AGL/glm/gtx/string_cast.hpp"
#include<thread>
#include<chrono>
#include "../AGL/stb_image.h"

double lastTime;
int nbFrames = 0;
void showFPS(GLFWwindow *window)
{
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if(delta >= 1)
    {
        double fps = double(nbFrames) / delta;
        std::stringstream ss;
        ss << fps << " FPS";
        glfwSetWindowTitle(window, ss.str().c_str());
        nbFrames = 0;
        lastTime = currentTime;
    }
}

int main_()
{
    stbi_set_flip_vertically_on_load(true);
    agl::Scene scene(640, 480);
    agl::Entity cube = agl::cube(true, true), plane = agl::plane(10, 10, true, true);
    agl::Light light(glm::vec3(2), 1, 1, 1);
    light.ambient *= 0.1;
//agl::loadObj("../models/teapot.obj");

    cube.material.createTexture("../texture/rough_wood_1.jpg");
    cube.material.customShader = true;
    plane.position.y = -1;
    plane.material.createTexture("../texture/leaves_1.jpg");

//    scene.add(plane);
    scene.add(light);
    scene.add(cube);

    scene.enableLights();
    scene.prepare();
    auto shader = cube.material.createShader(&cube, scene.lights);
    shader.second = agl::readTextFile("../shaders/glow.fsh");
    cube.material.setShader(shader.first, shader.second);
    int i = 0;
    glEnable(GL_CULL_FACE);
    while(glfwWindowShouldClose(scene.window) == 0)
    {
        cube.rotate(0.01, glm::vec3(0, 1, 0));
        cube.material.emission = glm::vec4(1, 0, 1, 1) * float(glm::sin(glfwGetTime()*5)*.5+.5);
        scene.camera.rotateY(-0.01);
        scene.render();
//        std::stringstream path;
//        path << "../img/img" << i++ << ".ppm";
//        agl::saveImage(path.str().c_str(), 640, 480);
//        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }

    return 0;
}
