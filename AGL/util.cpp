#include "util.h"
#include<sstream>
#include<fstream>

namespace agl {
GLuint loadShaders(std::string vertShader, std::string fragShader)
{
    GLuint vsID = glCreateShader(GL_VERTEX_SHADER),
           fsID = glCreateShader(GL_FRAGMENT_SHADER);
    GLint res = 1, logLen;

    const char *code = vertShader.c_str();
    glShaderSource(vsID, 1, &code, nullptr);
    glCompileShader(vsID);
    glGetShaderiv(vsID, GL_COMPILE_STATUS, &res);
    glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &logLen);
    if(!res)
    {
        char errMsg[logLen];
        glGetShaderInfoLog(vsID, logLen, nullptr, errMsg);
        fprintf(stderr, "Error in loading vertex shader.\n%s\n", errMsg);
        return 0;
    }

    code = fragShader.c_str();
    glShaderSource(fsID, 1, &code, nullptr);
    glCompileShader(fsID);
    glGetShaderiv(fsID, GL_COMPILE_STATUS, &res);
    glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &logLen);
    if(!res)
    {
        char errMsg[logLen];
        glGetShaderInfoLog(fsID, logLen, nullptr, errMsg);
        fprintf(stderr, "Error in loading fragment shader.\n%s\n", errMsg);
        return 0;
    }

    GLuint progID = glCreateProgram();
    glAttachShader(progID, vsID);
    glAttachShader(progID, fsID);
    glLinkProgram(progID);
    glGetProgramiv(progID, GL_LINK_STATUS, &res);
    glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &logLen);
    if(!res)
    {
        char errMsg[logLen];
        glGetProgramInfoLog(progID, logLen, nullptr, errMsg);
        fprintf(stderr, "Error in loading program.\n%s\n", errMsg);
        return 0;
    }
    glDetachShader(progID, vsID);
    glDetachShader(progID, fsID);
    glDeleteShader(vsID);
    glDeleteShader(fsID);
    return progID;
}
GLuint loadShadersFromFile(const char* vsPath, const char* fsPath)
{
    return loadShaders(readTextFile(vsPath), readTextFile(fsPath));
}
std::string readTextFile(const char *path)
{
    std::stringstream ss;
    std::ifstream f(path, std::ios::in);
    if(f.is_open())
    {
        ss << f.rdbuf();
        f.close();
    }
    return ss.str();
}
void saveImage(const char *path, int w, int h)
{
    std::ofstream fp(path);
    fp << "P6 " << std::to_string(w) << " " << std::to_string(h) << " 255 ";
    uint8_t data[h][w*3] = {0};
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, (void*)data);
    for(int i=h-1; i>=0; --i)
        for(int j=0, l=w*3; j<l; ++j)
            fp << data[i][j];
    fp.close();
}
}
