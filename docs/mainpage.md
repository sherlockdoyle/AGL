# AGL
AGL stands for auto-GL, a 3D graphics library written in C++. It is dedicated to creating 3D graphics with OpenGL while abstracting the complex parts.

## Dependency
AGL depends on
- OpenGL version 3.0 or above (3.3 used as default)
- GLFW (needs installation)
- GLM (provided)

AGL intends to be a low requirements library. You'll only need to install GLFW to get it working.

## Getting Started
Start by installing [GLFW](https://www.glfw.org/) for your system. Clone or download and extract the [repository](https://github.com/sherlockdoyle/AGL). You'll find all the header and source codes in the `AGL` directory. The compiled library will be in `build`. Add them to your build system - you can either add the header files and the library for linking or just the headers and the source codes for recompilation.

### First code
Several examples are available in the `test` folder. For this first code we'll create a new file inside this this folder. Start by creating a file `first_cube.cpp`, and add the following lines.
```.cpp
#include "../AGL/agl.h"  // Include the header file. This is all you need to get started.

int main()
{
	agl::Scene scene;  // Create your first scene. Everything in AGL is defined in the agl namespace.

    agl::Entity cube = agl::cube();                  // Create a cube to display.
    cube.material.emission = glm::vec4(1, 0, 0, 1);  // Add some red color to the cube. The color is in RGBA format.
    scene.add(cube);                                 // Add the cube to the scene.

	scene.prepare();        // You don't need to understand anything about this.
    while(scene.render());  // Render the scene. To close the window, press Esc.

	return 0;
}
```
Once you've saved the file, open a terminal and navigate to this folder. Compile the file with (assuming you've GCC installed)
```
g++ first_cube.cpp ../build/libAGL.a -lGL -lglfw
```
then run it with `./a.out`. Once the code runs without any errors, you'll see a red colored cube on a black background on the screen, like the image below.

![Red cube on a black background](img/mainpage_img1.png)

### Second code
In our first code we created a simple cube. But it had no depth, just looked like an irregular hexagon. Next, we'll add some lights, hence some depths. Create another file `lighted_cube.cpp` and add the following codes.
```.cpp
#include "../AGL/agl.h"  // Include the header file.

int main()
{
    agl::Scene scene;  // Create your second scene.

    agl::Entity cube = agl::cube();               // Create the cube.
    cube.material = agl::Material::green_rubber;  // This time we will not add any color. We'll directly use a predefined material.
    scene.add(cube);                              // Add the cube to the scene.

    agl::Light light(glm::vec3(-2, 3, 2),     // Create a white light positioned at (-2, 3, 2).
                     glm::vec4(1, 1, 1, 1));  // The second parameter is the color.
    scene.add(light);      // Add the light to the scene.
    scene.enableLights();  // We need to enable lights for lighting calculations.

    scene.prepare();        // Still don't need to understand.
    while(scene.render());  // Render the scene. You know how to close the window.

    return 0;
}
```
Compile and run this code. You should see something like the image below.

![Green rubber cube with light](img/mainpage_img2.png)

### What next?
Add the following line inside the `while` loop and create your first animation.
```
scene.camera.rotateY(0.01);
```
**Then?** Check the tutorials for more awesomeness.