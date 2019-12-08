#include "util.h"
#include<sstream>
#include<fstream>

GLuint agl::loadShaders(const char* vertShader, const char* fragShader)
{
    GLuint vsID = glCreateShader(GL_VERTEX_SHADER),
           fsID = glCreateShader(GL_FRAGMENT_SHADER);
    GLint res = GL_FALSE;
    int logLen;

    glShaderSource(vsID, 1, &vertShader, nullptr);
    glCompileShader(vsID);
    glGetShaderiv(vsID, GL_COMPILE_STATUS, &res);
    glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &logLen);
    if(!res || logLen > 0)
    {
        char errMsg[logLen + 1];
        glGetShaderInfoLog(vsID, logLen, nullptr, &errMsg[0]);
        printf("Error in loading vertex shader.\n%s\n", errMsg);
    }

    glShaderSource(fsID, 1, &fragShader, nullptr);
    glCompileShader(fsID);
    glGetShaderiv(fsID, GL_COMPILE_STATUS, &res);
    glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &logLen);
    if(!res || logLen > 0)
    {
        char errMsg[logLen + 1];
        glGetShaderInfoLog(vsID, logLen, nullptr, &errMsg[0]);
        printf("Error in loading fragment shader.\n%s\n", errMsg);
    }

    GLuint progID = glCreateProgram();
    glAttachShader(progID, vsID);
    glAttachShader(progID, fsID);
    glLinkProgram(progID);
    glGetProgramiv(progID, GL_LINK_STATUS, &res);
    glGetShaderiv(progID, GL_INFO_LOG_LENGTH, &logLen);
    if(!res || logLen > 0)
    {
        char errMsg[logLen + 1];
        glGetProgramInfoLog(progID, logLen, nullptr, &errMsg[0]);
        printf("Error in loading program.\n%s\n", errMsg);
    }
    glDetachShader(progID, vsID);
    glDetachShader(progID, fsID);
    glDeleteShader(vsID);
    glDeleteShader(fsID);
    return progID;
}

GLuint agl::loadShadersFromFile(const char* vsPath, const char* fsPath)
{
    std::string vertShader, fragShader;
    std::ifstream fv(vsPath, std::ios::in);
    if(fv.is_open())
    {
        std::stringstream ss;
        ss << fv.rdbuf();
        vertShader = ss.str();
        fv.close();
    }
    std::ifstream ff(fsPath, std::ios::in);
    if(ff.is_open())
    {
        std::stringstream ss;
        ss << ff.rdbuf();
        fragShader = ss.str();
        ff.close();
    }
    return loadShaders(vertShader.c_str(), fragShader.c_str());
}

void agl::saveImage(const char *path, int w, int h)
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
