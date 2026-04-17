#include <iostream>
#include <cmath>
#include "../includes/Mat4.hpp"
#include "../includes/App.hpp"

App::App()
    : _window(NULL), _width(1280), _height(720), _title("scop"), _vao(0), _vbo(0)
{
}

App::~App()
{
    cleanup();
}

bool App::initTriangle()
{
    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
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

    if (!initTriangle())
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
    float time = glfwGetTime();
    Mat4 translation = Mat4::translation(0.0f, 0.0f, 0.0f);
    Mat4 rotation = Mat4::rotationZ(time);
    Mat4 model = translation * rotation;

    while (!glfwWindowShouldClose(_window))
    {
        time = glfwGetTime();
        translation = Mat4::translation(0.0f, 0.0f, 0.0f);
        rotation = Mat4::rotationZ(time);
        model = translation * rotation;

        processInput();

        glClearColor(0.12f, 0.12f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _shader.use();
        GLuint modelLoc = glGetUniformLocation(_shader.getProgram(), "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.m);
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

void App::cleanup()
{
    if (_vbo != 0)
    {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }

    if (_vao != 0)
    {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }

    _shader.destroy();

    if (_window)
    {
        glfwDestroyWindow(_window);
        _window = NULL;
    }

    glfwTerminate();
}