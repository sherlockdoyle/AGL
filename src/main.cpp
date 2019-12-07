#include<GLES3/gl32.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<sstream>
#include "agl.h"

//int main()
//{
//    GLFWwindow* window;
//    if(!glfwInit())
//        return 1;

//    glfwWindowHint(GLFW_SAMPLES, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
////    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
////    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//    window = glfwCreateWindow(640, 480, "Hello", nullptr, nullptr);
//    if(!window)
//    {
//        glfwTerminate();
//        return 1;
//    }
//    glfwMakeContextCurrent(window);
////    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

//    glClearColor(0, 0, 0, 0);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);

//    GLuint pID = agl::loadShadersFromFile("../vert.vsh", "../frag.fsh");
//    glUseProgram(pID);
//    GLuint mID = glGetUniformLocation(pID, "MVP");
//    glm::mat4 projection = glm::perspective(glm::radians(45.f), 4.f/3.f, 0.1f, 100.f);
//    glm::mat4 view = glm::lookAt(glm::vec3(1,0,1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
//    for(int i=0; i<4; ++i)
//        std::cout << view[i][0] << " " << view[i][1] << " " << view[i][2] << " " << view[i][3] << std::endl;
//    glm::mat4 model = glm::mat4(1);
//    glm::mat4 mvp = projection * view * model;

//    GLfloat verts[] = {-1, -1, -1,
//                       -1, -1,  1,
//                       -1,  1, -1,
//                       -1,  1,  1,
//                        1, -1, -1,
//                        1, -1,  1,
//                        1,  1, -1,
//                        1,  1,  1};
//    GLuint indices[] = {2, 0, 1,  // left
//                        2, 1, 3,
//                        4, 5, 7,  // right
//                        4, 7, 6,
//                        0, 4, 5,  // bottom
//                        0, 5, 1,
//                        2, 7, 6,  // top
//                        2, 3, 7,
//                        0, 6, 4,  // back
//                        0, 2, 6,
//                        1, 5, 7,  // front
//                        1, 7, 3};

//    GLuint vertArrID, vertBuff, idxBuff;
//    glGenVertexArrays(1, &vertArrID);
//    glGenBuffers(1, &vertBuff);
//    glGenBuffers(1, &idxBuff);
//    glBindVertexArray(vertArrID);
//    glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBuff);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

//    float i=0.01;
//    int c=0;
//    glm::vec3 pos = glm::vec3(0,0,5);
//    glm::vec3 at = glm::vec3(0,0,0);
////    view = glm::lookAt(pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
//    do
//    {
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        pos = glm::rotate(pos-at, i, glm::vec3(1,0,0))+at;
////        std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
//        view = glm::lookAt(pos, at, glm::vec3(0, glm::sign(pos.z), 0));
////        model = glm::rotate(model, i, glm::vec3(1,1,1));
//        mvp = projection * view * model;
//        glUniformMatrix4fv(mID, 1, GL_FALSE, &mvp[0][0]);
//        glEnableVertexAttribArray(0);
////        glDrawArrays(GL_TRIANGLES, 0, 12*3);
//        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);
//        glDisableVertexAttribArray(0);

//        glfwSwapBuffers(window);
//        glfwPollEvents();
////        if(c++<100)
////        {
////            std::stringstream ss;
////            ss<<"../img/img"<<c<<".ppm";
////            agl::saveImage(ss.str().c_str(), 640, 480);
////        }
//    }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

//    glfwTerminate();
//    return 0;
//}
double lastTime;
int nbFrames = 0;
void showFPS(GLFWwindow *pWindow)
{
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if(delta >= 1)
    {
        double fps = double(nbFrames) / delta;
        std::stringstream ss;
        ss << fps << " FPS";
        glfwSetWindowTitle(pWindow, ss.str().c_str());
        nbFrames = 0;
        lastTime = currentTime;
    }
}

int main()
{
    agl::Scene scene(640, 480);
    scene.camera.lookAt(-1,0,0);
    agl::Entity cube = agl::unitCube(), c2 = agl::unitSphere(40, 40);
    cube.material.ambient.r = 1;
    c2.material.ambient = glm::vec4(-1, -1, -1, AGL_COLOR_POS2RGB_NORMED);
    scene.add(cube);
    scene.add(c2);
    c2.translate(-1,0,0);
    c2.scale(2,2,2);
    scene.prepare();
    int i=0;
    while(glfwWindowShouldClose(scene.window) == 0)
    {
        cube.rotate(0.01, glm::vec3(1, 0, 0));
        c2.rotate(-0.01, glm::vec3(2, 3, 5));
        scene.render();
//        std::stringstream path;
//        path << "../img/img" << i++ << ".ppm";
//        agl::saveImage(path.str().c_str(), 640, 480);
    }

    return 0;
}
