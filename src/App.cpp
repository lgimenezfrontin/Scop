#include <iostream>
#include <cmath>
#include "../includes/Mat4.hpp"
#include "../includes/App.hpp"

App::App()
    : _window(NULL), _width(1280), _height(720), _title("scop")
{
}

App::~App()
{
    cleanup();
}

bool App::initMesh()
{
    std::vector<Vertex> vertices;

    if (!ObjParser::load("assets/test.obj", vertices))
        return false;

    return _mesh.upload(vertices);
}

bool App::init()
{
    if (!glfwInit())
    {
        std::cerr << "Error: failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
    if (!_window)
    {
        std::cerr << "Error: failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(_window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error: failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(_window);
        _window = NULL;
        glfwTerminate();
        return false;
    }

    glViewport(0, 0, _width, _height);

    if (!_shader.loadFromFiles("shaders/triangle.vert", "shaders/triangle.frag"))
        return false;

    if (!initMesh())
        return false;

    return true;
}

void App::processInput()
{
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

void App::run()
{
    while (!glfwWindowShouldClose(_window))
    {
        float time = glfwGetTime();

        Mat4 rotation = Mat4::rotationZ(time);
        Mat4 placeInScene = Mat4::translation(0.0f, 0.0f, -2.0f);

        Mat4 model = placeInScene * rotation;

        Mat4 view = Mat4::identity();

        float aspect = static_cast<float>(_width) / static_cast<float>(_height);
        Mat4 projection = Mat4::perspective(45.0f * 3.14159265f / 180.0f, aspect, 0.1f, 100.0f);

        processInput();

        glClearColor(0.12f, 0.12f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _shader.use();

        GLuint modelLoc = glGetUniformLocation(_shader.getProgram(), "model");
        GLuint viewLoc = glGetUniformLocation(_shader.getProgram(), "view");
        GLuint projectionLoc = glGetUniformLocation(_shader.getProgram(), "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.m);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.m);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.m);

        _mesh.draw();

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

void App::cleanup()
{
    _mesh.destroy();
    _shader.destroy();

    if (_window)
    {
        glfwDestroyWindow(_window);
        _window = NULL;
    }

    glfwTerminate();
}