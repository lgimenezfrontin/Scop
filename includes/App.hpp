#ifndef APP_HPP
#define APP_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Shader.hpp"

class App
{
public:
    App();
    ~App();

    bool init();
    void run();
    void cleanup();

private:
    GLFWwindow* _window;
    int _width;
    int _height;
    std::string _title;

    Shader _shader;
    GLuint _vao;
    GLuint _vbo;

    void processInput();
    bool initTriangle();
};

#endif