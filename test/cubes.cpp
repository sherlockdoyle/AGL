#include<ctime>
#include "../AGL/agl.h"

int main_()
{
    std::srand(std::time(0));  // set random seed
    std::vector<agl::Material> materials{agl::Material::emerald, agl::Material::jade, agl::Material::obsidian,
                agl::Material::pearl, agl::Material::ruby, agl::Material::turquoise, agl::Material::brass,
                agl::Material::bronze, agl::Material::chrome, agl::Material::copper, agl::Material::gold,
                agl::Material::silver, agl::Material::black_plastic, agl::Material::cyan_plastic,
                agl::Material::green_plastic, agl::Material::red_plastic, agl::Material::white_plastic,
                agl::Material::yellow_plastic, agl::Material::black_rubber,  agl::Material::cyan_rubber,
                agl::Material::green_rubber, agl::Material::red_rubber, agl::Material::white_rubber,
                agl::Material::yellow_rubber};  // all the predefined materials

    agl::Scene scene;  // create the scene

    agl::Light light(glm::vec3(2,3,5), glm::vec4(1));  // create a white light at (5, 5, 5)
    light.ambient *= 0.2;  // decrease the ambient effect
    scene.add(light);  // add the light to the scene

    int n = 5;  // number of cubes
    std::vector<agl::Entity> cubes {};  // needed to store the cubes
    for(int i=0; i<n; ++i)
        for(int j=0; j<n; ++j)
        {
            agl::Entity cube = agl::cube(true);  // create a cube with normals
            cube.position = glm::vec3(2*i-n+1, 0, 2*j-n+1);  // set the position
            cube.material = materials[std::rand()%materials.size()];  // set the material; this is inefficient since
                                                                      // every cube gets a new shader
            cubes.push_back(cube);  // store the cube
        }
    for(agl::Entity &cube: cubes)
        scene.add(cube);  // add all the cubes to the scene

    scene.enableLights();  // enable lighting shaders
    scene.prepare();  // prepare the scene (build the shaders, VBOs etc.)
    while(glfwWindowShouldClose(scene.window) == 0)
    {
        scene.camera.rotateY(-0.01);  // rotate the camera around the scene
        scene.render();  // render the scene (display everything on the screen)
    }

    return 0;
}
