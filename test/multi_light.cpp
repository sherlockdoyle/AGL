#include "../AGL/agl.h"

int main_multi_light()
{
    agl::Scene scene(640, 480);
    scene.camera.setPos(-7,1,7);
    agl::Entity cube1 = agl::cube(true), cube2 = agl::cube(true), cube3 = agl::cube(true),
            plane = agl::plane(100, 100, true),
            light1_holder = agl::cube(), light2_holder = agl::cube(), light3_holder = agl::cube();
    plane.material = agl::Material::white_plastic + agl::Material::white_rubber;
    plane.position.y = -1.5;

    agl::Light light1(glm::vec3(0), 0, 0, 1),
               light2(glm::vec3(0), 0, 1, 0),
               light3(glm::vec3(0), 1, 1, 1);
    light1.ambient *= 0.5;
    light2.ambient *= 0.5;
    light3.ambient *= 0.5;
    light2.specular *= .1;

    light1_holder.scale(0.4);
    light2_holder.scale(0.4);
    light3_holder.scale(0.4);
    light1_holder.add(light1);
    light2_holder.add(light2);
    light3_holder.add(light3);
    light1_holder.material.ambient = light1.diffuse;
    light2_holder.material.ambient = light2.diffuse;
    light3_holder.material.ambient = light3.diffuse;
    light1_holder.position = glm::vec3(1, .75, 4);
    light2_holder.position = glm::vec3(1, 3, -1);
    light3_holder.position = glm::vec3(-3, -1, 2);

    cube1.material = agl::Material::brass;
    cube2.material = agl::Material::bronze;
    cube3.material = agl::Material::pearl;
    cube1.translate(0,0,2);
    cube2.scale(.75);
    cube2.translate(-1, 2, 0);
    cube3.rotate(glm::radians(-30.f), glm::vec3(2,3,5));
    cube3.translate(-2, -1, -2);

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
    scene.prepare();
    while(glfwWindowShouldClose(scene.window) == 0)
    {
        scene.camera.rotateY(-0.01);
        scene.render();
    }

    return 0;
}
