

# AGL
3D graphics library

## What is AGL?
[AGL](https://sherlockdoyle.github.io/AGL) stands for auto-GL (or abstract-GL, or another-GL, or ..., whatever you prefer), a 3D graphics library written in C++. It is dedicated to creating 3D graphics with OpenGL by abstracting the complex parts. This library is intended for those who're learning OpenGL, or are just fascinated with real-like objects moving on the screen.

## How do I use this?
AGL is dependent on [OpenGL](https://en.wikipedia.org/wiki/OpenGL), [GLFW](https://www.glfw.org/) and [glm](https://github.com/g-truc/glm). Chances are OpenGL is already available on your system, just make sure that you've at least version 3.0. You might need to install GLFW from [here](https://www.glfw.org/download.html). As of glm, the library is included. Check the [tutorials]() and [documentations](https://sherlockdoyle.github.io/AGL/html/index.html) for usage.

### But how do I actually use it?
The simplest way to use it (or rather experiment) would be to run the example files in [`test`](/test).
- Download (or [clone](https://help.github.com/en/github/creating-cloning-and-archiving-repositories/cloning-a-repository)) this repository by clicking the green button, or [here](https://github.com/sherlockdoyle/AGL/archive/master.zip).
- Extract the files and go to the [`test`](/test) directory. Every file here (except `main.cpp`) is an example.
- Open the C++ file that you want to run. You'll find a function named `main_<name of file>`. Change this to just `main`, save and close the file.
  #### Why all this fuss?
  The IDE that I use (in fact any IDE) do not like more than one `main` function in a project. But as long as you're using the command line, this should be fine.
- Open a terminal in this directory and compile the file with this command
  ```
  g++ <name of file> ../build/libAGL.a -lGL -lglfw
  ```
  #### What's all these?
  - `g++` refers to the GNU C compiler. Get it [here](https://gcc.gnu.org/).
  - Replace `<name of file>` with the file name that you want to compile.
  - `../build/libAGL.a` is the path to the compiled library.
  - `-lGL` lets the compiler know that we're using OpenGL functions.
  - `-lglfw` lets the compiler know that we're using GLFW.
  #### But I don't use GCC.
  If you're using any other compiler, probably they'll have similar commands too.
- Run the compiled `a.out` file with (on Linux)
  ```
  ./a.out
  ```
- If there were any errors during compilation or execution, it means you might be missing a library or the default OpenGL version is not supported.

## How do I use this with an IDE?
To use this with an IDE
- Start by creating a new C++ project, in case you don't already have one.
- Search around for an option similar to '*Add library*' in your IDE. Click it. If you can't find this option, go to project settings/properties and configure the compiler and linker options.
- Add the library file, `build/libAGL.a`.
- Add the include directory, `AGL`.
- Add `-lGL` and `-lglfw` library.

## Why'd I use this?
You don't need to. In fact, if you want to build a performance intensive program (say, for games), you might wanna check some other library. This library is intended for beginners and experimentation.

## Why this library?
Trying to learn OpenGL for the first time can be pretty confusing. There's a lot of setups required with a lot of libraries. Different tutorials will ask you to install different libraries, including GLFW, GLU, GLUT, GLM etc. These tutorials also mixes window creation, the actual OpenGL program, shaders etc. together. I personally feel that these are unnecessary and the user should concentrate on a single part of the program and let this library abstract away the complexities.

Another reason for this library is to create 3D images/videos quickly. Using dedicated software, like Blender, for this task might be time consuming and you might not need the extra details. In fact, having to learn a new software might not be as efficient when you can code, and explain your structure better with code. However, trying to write all the codes for a simple image, including the math etc. is an overkill.

Thus, enters this library, taking away the pain of writing all the unnecessary codes. It also tries to reduce the number of installations - you only need to install GLFW, glm is available with the library. *In fact, the dependence on GLFW might be removed in future.*

## Why is everything (how do I say this) so non-technical around here?
Because this library is for beginners; more importance is given to simplicity than technicality.

## Why isn't there a Makefile?
The examples in [`test`](/test) directory are intended for learning and the commands needed for compiling them should be understood instead of relying on make. Moreover, Makefile requires the installation if make and installations are something that this library tries to reduce. As of the actual library, if you *need* to recompile it, it'd be better to add the source to a project and edit them there.
