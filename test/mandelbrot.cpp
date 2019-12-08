#include<iostream>
#include<GLES3/gl32.h>
#include<GLFW/glfw3.h>
#include "util.h"

int width = 800, height = 600, itr = 200, shaderProgram;
double zoom = 100, dx = 0, dy = 0, oldx, oldy;
bool dragging = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if(key == GLFW_KEY_KP_0 && action == GLFW_PRESS)
    {
        itr = 200;
        zoom = 100.0;
        dx = 0.0;
        dy = 0.0;
    }
    if(key == GLFW_KEY_A && action == GLFW_PRESS)
        dx += 20 / zoom;
    else if(key == GLFW_KEY_D && action == GLFW_PRESS)
        dx -= 20 / zoom;
    else if(key == GLFW_KEY_W && action == GLFW_PRESS)
        dy -= 20 / zoom;
    else if(key == GLFW_KEY_S && action == GLFW_PRESS)
        dy += 20 / zoom;
    if(key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
        zoom *= 2;
    else if(key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
        zoom /= 2;
    if(key == GLFW_KEY_Q && action == GLFW_PRESS)
        itr += 50;
    else if(key == GLFW_KEY_E && action == GLFW_PRESS)
        (itr > 100) ? itr -= 50 : itr = 50;
    glUniform1i(glGetUniformLocation(shaderProgram, "itr"), itr);
    glUniform1f(glGetUniformLocation(shaderProgram, "zoom"), zoom);
    glUniform2f(glGetUniformLocation(shaderProgram, "dxy"), dx, dy);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &oldx, &oldy);
        dragging = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        dragging = false;
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(dragging)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        dx += (xpos - oldx) / zoom;
        dy += (oldy - ypos) / zoom;
        oldx = xpos;
        oldy = ypos;
        glUniform2f(glGetUniformLocation(shaderProgram, "dxy"), dx, dy);
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(yoffset != 0)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        double dx_ = (xpos - width / 2) / zoom - dx;
        double dy_ = (height - ypos - height / 2) / zoom - dy;
        dx = -dx_;
        dy = -dy_;
        if(yoffset < 0)
            zoom /= 1.2;
        else
            zoom *= 1.2;
        dx_ = (xpos - width / 2) / zoom;
        dy_ = (height - ypos - height / 2) / zoom;
        dx += dx_;
        dy += dy_;
        glUniform1f(glGetUniformLocation(shaderProgram, "zoom"), zoom);
        glUniform2f(glGetUniformLocation(shaderProgram, "dxy"), dx, dy);
    }
}
void window_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, width, height);
    width = w;
    height = h;
    glUniform2f(glGetUniformLocation(shaderProgram, "screenSize"), (double)width, (double)height);
}

int main()
{
    GLFWwindow* window;
    if(!glfwInit())
        return 1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Mandelbrot", nullptr, nullptr);
    if(!window)
    {
        glfwTerminate();
        return 1;
    }

    glClearColor(0, 0, 0, 0);
    glfwSetErrorCallback([](int e, const char* s){std::cout << s << std::endl;});
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwMakeContextCurrent(window);

    GLuint vertArrID;
    glGenVertexArrays(1, &vertArrID);
    glBindVertexArray(vertArrID);

    shaderProgram = agl::loadShadersFromFile("../mandelbrot_vert.vsh", "../mandelbrot_frag.fsh");
    glUseProgram(shaderProgram);
    glUniform2f(glGetUniformLocation(shaderProgram, "screenSize"), width, height);
    glUniform2f(glGetUniformLocation(shaderProgram, "dxy"), dx, dy);
    glUniform1f(glGetUniformLocation(shaderProgram, "zoom"), zoom);
    glUniform1i(glGetUniformLocation(shaderProgram, "itr"), itr);

    GLfloat verts[] = {-1.0f,-1.0f,
                        1.0f,-1.0f,
                        1.0f, 1.0f,
                        1.0f, 1.0f,
                       -1.0f, 1.0f,
                       -1.0f,-1.0f};
    GLuint vertBuff;
    glGenBuffers(1, &vertBuff);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
