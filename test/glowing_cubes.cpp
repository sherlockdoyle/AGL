#include<ctime>
#include "../AGL/agl.h"
#include "../AGL/glm/gtc/random.hpp"
#include "../AGL/glm/gtc/matrix_transform.hpp"

int main()
{
    std::srand(std::time(0));  // set random seed
    agl::Scene scene;  // create the scene
    scene.camera.setPos(15,5,15);  // set the camera position
    agl::Light light(glm::vec3(0), 1, 1, 1);  // create a white light
    light.ambient *= 0.1;  // set light parameters
    light.specular *= .5;
    light.quadraticAttenuation = 0.075;
    agl::Entity lightCube = agl::cube();  // create a visual for the light
    lightCube.add(light);
    lightCube.position = glm::vec3(-2, -2, 2);  // set cube parameters
    lightCube.scale(.15);
    lightCube.material.emission = light.diffuse;
    scene.add(lightCube);  // add the cube, and the light hence, to the scene

    std::vector<agl::Entity> cubes{agl::cube(true, true)};  // will store all the cubes
    std::vector<glm::vec3> axes{glm::ballRand(1.f)};  // will store the axis for rotationn
    std::vector<glm::vec2> rads{glm::circularRand(1.f)};  // will store the speed of rotation
    float colors[] = {1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,1,1,.65,0,.5,1,0,0,.5,1,0,1,.5};  // glow color for each cube
    cubes[0].material.createTexture("../texture/rough_wood.jpg");  // set texture for first cube
    cubes[0].material.customShader = true;  // custom shader needs to be set manually
    for(int i=1; i<10; ++i)
    {
        cubes.push_back(agl::cube(true, true));  // create all the cubes
        cubes[i].material.customShader = true;
        cubes[i].position = glm::ballRand(10.f);  // set them at a random position
        cubes[i].position.y *= .67;
        axes.push_back(glm::ballRand(1.f));  // add a random axis of rotation
        rads.push_back(glm::circularRand(1.f));  // add a random speed for rotation
    }

    for(agl::Entity &cube: cubes)
        scene.add(cube);  // add all the cubes
    scene.enableLights();  // enable lighting calculations
    lightCube.material.lightsEnabled = false;  // don't want the light cube lighted up
    scene.prepare();  // prepare the scene

    auto shader = cubes[0].material.createShader(&cubes[0], scene.lights);  // create the default shader for the first cube
    cubes[0].material.setShader(shader.first, agl::readTextFile("../shaders/glowing_cubes.fsh"));  // set the default vertex shader and a custom fragment shader
    for(int i=1; i<10; ++i)
        cubes[i].material = cubes[0].material;  // all other cubes will share the first cubes material

    while(glfwWindowShouldClose(scene.window) == 0)
    {
        for(int i=0, l=cubes.size(); i<l; ++i)  // just some animations
        {
            cubes[i].rotate(rads[i].x/50, axes[i]);
            cubes[i].material.emission = glm::vec4(colors[i*3], colors[i*3+1], colors[i*3+2], 1) * float(glm::sin(glfwGetTime()*5*rads[i].y)*.5+.5);
        }
        scene.camera.rotateY(-0.005);
        scene.render();
    }

    return 0;
}
