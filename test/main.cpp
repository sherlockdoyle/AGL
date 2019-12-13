#include<sstream>
#include "../AGL/agl.h"
#include "../AGL/glm/gtx/string_cast.hpp"
#include<thread>
#include<chrono>

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

int main()
{
    agl::Scene scene(640, 480);
    scene.camera.setPos(-7,1,7);
    agl::Entity cube1 = agl::cube(true), cube2 = agl::cube(true), cube3 = agl::cube(true),
            plane = agl::plane(100, 100, true),
            light1_holder = agl::cube(), light2_holder = agl::cube(), light3_holder = agl::cube();
    plane.material = agl::Material::white_plastic + agl::Material::white_rubber;
    plane.position.y = -1.5;
    light1_holder.scale(0.4);
    light2_holder.scale(0.4);
    light3_holder.scale(0.4);

    agl::Light light1(glm::vec3(0), 0, 0, 1),
               light2(glm::vec3(0), 0, 1, 0),
               light3(glm::vec3(0), 1, 1, 1);
    light1.ambient *= 0.5;
    light2.ambient *= 0.5;
    light3.ambient *= 0.5;
    light2.specular *= .1;
    light1_holder.add(light1);
    light2_holder.add(light2);
    light3_holder.add(light3);
    light1_holder.material.ambient = light1.diffuse;
    light2_holder.material.ambient = light2.diffuse;
    light3_holder.material.ambient = light3.diffuse;

    cube1.material = agl::Material::brass;
    cube2.material = agl::Material::bronze;
    cube3.material = agl::Material::pearl;

    cube1.translate(0,0,2);
    cube2.scale(.75);
    cube2.translate(-1, 2, 0);
    cube3.rotate(glm::radians(-30.f), glm::vec3(2,3,5));
    cube3.translate(-2, -1, -2);
    light1_holder.position = glm::vec3(1, .75, 4);
    light2_holder.position = glm::vec3(1, 3, -1);
    light3_holder.position = glm::vec3(-3, -1, 2);

    scene.add(plane);
    scene.add(light1_holder);
    scene.add(light2_holder);
    scene.add(light3_holder);
    scene.add(cube1);
    scene.add(cube2);
    scene.add(cube3);

    scene.enableLights();
    light1_holder.material.lightsEnabled = false;
    light2_holder.material.lightsEnabled = false;
    light3_holder.material.lightsEnabled = false;
//    std::cout << glm::to_string(light1.getPos()) << std::endl;
    scene.prepare();
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glLineWidth(3);
//    int i = 0;
    while(glfwWindowShouldClose(scene.window) == 0)
    {
//        cube1.rotate(0.01, glm::vec3(0, 1, 0));
        scene.camera.rotateY(-0.01);
        scene.render();
//        std::stringstream path;
//        path << "../img/img" << i++ << ".ppm";
//        agl::saveImage(path.str().c_str(), 640, 480);
//        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }

    return 0;
}
