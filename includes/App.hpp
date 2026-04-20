#ifndef APP_HPP
#define APP_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Shader.hpp"
#include "Mesh.hpp"
#include "ObjParser.hpp"
#include "Vec3.hpp"

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
    Mesh _mesh;
    Vec3 _objectPosition;

    void processInput();
    bool initMesh();
};

#endif