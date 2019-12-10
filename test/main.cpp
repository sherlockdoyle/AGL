#include<sstream>
#include "../AGL/agl.h"

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
    agl::Entity cube = agl::cube(true),
            cube2 = agl::sphere(),
            plane = agl::plane(10, 10);
    plane.material = agl::Material::ruby;
    plane.position.y = -1.5;

    agl::Light sunlight(glm::vec3(2, 2, 2), 1, 1, 1);
    sunlight.ambient *= 0.5;
//    sunlight.specular *= 0.5;

    cube.material = agl::Material::green_rubber;
    cube2.material.ambient = glm::vec4(1);
    cube2.scale(0.1);
    cube2.position = sunlight.position;

    scene.add(plane);
    scene.add(cube2);
    scene.add(sunlight);
    scene.add(cube);
    scene.enableLights();
//    plane.material.lightsEna/bled = false;
    cube2.material.lightsEnabled = false;
    scene.prepare();
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glLineWidth(3);
    while(glfwWindowShouldClose(scene.window) == 0)
    {
        cube.rotate(0.01, glm::vec3(0, 1, 0));
        scene.camera.rotateY(-0.005);
        scene.render();
//        std::stringstream path;
//        path << "../img/img" << i++ << ".ppm";
//        agl::saveImage(path.str().c_str(), 640, 480);
    }

    return 0;
}
