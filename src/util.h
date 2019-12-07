#ifndef UTIL_H
#define UTIL_H

#include<GLES3/gl32.h>
#include<iostream>

namespace agl {
GLuint loadShaders(const char* vertShader, const char* fragShader);
GLuint loadShadersFromFile(const char* vsPath, const char* fsPath);
void saveImage(const char* path, int w, int h);
}

#define AGL_PI 3.141592653589793238462643383279502884197169399375105820974

#define AGL_GLFW_INIT_ERROR 1
#define AGL_GLFW_CREATE_WINDOW_ERROR 2

#define AGL_SAMPLE_SIZE 4
#define AGL_GLVERSION_MAJOR 4
#define AGL_GLVERSION_MINOR 5

#define AGL_POINTS 1
#define AGL_LINES 2
#define AGL_TRIGS 4

#define AGL_SHADING_FLAT 1
#define AGL_SHADING_GOURAUD 2
#define AGL_SHADING_PHONG 3

#define AGL_COLOR_POS2RGB_NORMED -1

#endif // UTIL_H
