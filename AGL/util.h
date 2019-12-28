#ifndef UTIL_H
#define UTIL_H

#include<GLES3/gl32.h>
#include<iostream>

namespace agl {
/*!
 * \brief Load a pair of shaders.
 * \param vertShader The vertex shader.
 * \param fragShader The fragment shader.
 * \return The program ID after loading the shaders.
 *
 * This function takes in two strings consisting a vertex and a fragment shader, compiles them and creates a program. It
 * loads the program in the current OpenGL context and returns the program ID which can be used to access the shaders.
 */
GLuint loadShaders(std::string vertShader, std::string fragShader);
/*!
 * \brief Load shaders from files.
 * \param vsPath Path to the vertex shader.
 * \param fsPath Path to the fragment shader.
 * \return The program ID after loading the shaders.
 *
 * This function loads the vertex and fragment shaders from two files and loads them with #loadShaders.
 */
GLuint loadShadersFromFile(const char* vsPath, const char* fsPath);
/*!
 * \brief Reads a text file.
 * \param path Path to the file.
 * \return The contents of the file as a string.
 */
std::string readTextFile(const char* path);
/*!
 * \brief Save the current render as a ppm image.
 * \param path Path to save the image as.
 * \param w Width of the image.
 * \param h Height of the image.
 *
 * This function saves the render of the current OpenGL context as a [PPM](https://my.eng.utah.edu/~cs5610/ppm.html) P6
 * file. You can use PPM viewer or converter to view the image. [ffmpeg](https://www.ffmpeg.org/) has support for PPM.
 */
void saveImage(const char* path, int w, int h);
}

#define AGL_PI 3.141592653589793238462643383279502884197169399375105820974  //!< [\f$\pi\f$](https://en.wikipedia.org/wiki/Pi). What else?

/*!
 * \name Internal constants
 * These values are used by AGL internally while creating the GLFW window.
 * @{
 */
#define AGL_SAMPLE_SIZE 4  //!< Samples to take per pixel for [anti-aliasing](https://en.wikipedia.org/wiki/Anti-aliasing).
#define AGL_GLVERSION_MAJOR 4  //!< OpenGL major version.
#define AGL_GLVERSION_MINOR 5  //!< OpenGL minor version.

#define AGL_GLFW_INIT_ERROR 1  //!< Error if GLFW was not initialised.
#define AGL_GLFW_CREATE_WINDOW_ERROR 2  //!< Error if window was not created.
/*! @}*/

/*!
 * \name Types of shading
 * These values are used to define how entities will be shaded. **These are currently unused.**
 * @{
 */
#define AGL_POINTS 1  //!< Display each vertex as a point.
#define AGL_LINES 2  //!< Display each edge as a line.
#define AGL_TRIGS 4  //!< Display each face; this is default.

#define AGL_SHADING_FLAT 1  //!< Each face will be shaded with a single color.
#define AGL_SHADING_GOURAUD 2  //!< Each vertex will be shaded.
#define AGL_SHADING_PHONG 3  //!< Each point (fragment) will be shaded.
/*! @}*/

/*!
 * \name Special colors
 * These are constants for special colors for #Material.
 * @{
 */
#define AGL_COLOR_POS2RGB -1  //!< Position based color.
#define AGL_COLOR_NORM2RGB -2  //!< Normal based color.
#define AGL_COLOR_CHECKER -3  //!< Checker board pattern.
/*! @}*/

#endif // UTIL_H
