#include <iostream>
#include <cmath>
#include "../includes/Mat4.hpp"
#include "../includes/App.hpp"

App::App()
    : _window(NULL),
      _width(1280),
      _height(720),
      _title("scop"),
      _objectPosition(0.0f, 0.0f, -6.0f),
      _blendFactor(0.0f),
      _textureEnabled(false),
      _toggleKeyPressed(false),
      _rotationKeyPressed(false),
      _recenterKeyPressed(false),
      _rotationAxis(0)
{
}

App::~App()
{
    cleanup();
}

bool App::initMesh()
{
    std::vector<Vertex> vertices;

    if (!ObjParser::load("assets/teapot2.obj", vertices))
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
    glEnable(GL_DEPTH_TEST);

    if (!_shader.loadFromFiles("shaders/triangle.vert", "shaders/triangle.frag"))
        return false;

    if (!initMesh())
        return false;

    if (!_texture.loadBMP("assets/Fish.bmp"))
        return false;

    return true;
}

void App::processInput()
{
    float moveSpeed = 0.02f;

    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, GLFW_TRUE);

    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        _objectPosition.x -= moveSpeed;
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        _objectPosition.x += moveSpeed;

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        _objectPosition.y += moveSpeed;
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        _objectPosition.y -= moveSpeed;

    if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
        _objectPosition.z += moveSpeed;
    if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
        _objectPosition.z -= moveSpeed;

    if (glfwGetKey(_window, GLFW_KEY_T) == GLFW_PRESS && !_toggleKeyPressed)
    {
        _toggleKeyPressed = true;
        _textureEnabled = !_textureEnabled;
    }
    else if (glfwGetKey(_window, GLFW_KEY_T) == GLFW_RELEASE)
        _toggleKeyPressed = false;

    if (glfwGetKey(_window, GLFW_KEY_R) == GLFW_PRESS && !_rotationKeyPressed)
    {
        _rotationKeyPressed = true;
        if (_rotationAxis == 0 || _rotationAxis == 1)
            _rotationAxis++;
        else
            _rotationAxis = 0;
    }
    else if (glfwGetKey(_window, GLFW_KEY_R) == GLFW_RELEASE)
        _rotationKeyPressed = false;

    if (glfwGetKey(_window, GLFW_KEY_C) == GLFW_PRESS && !_recenterKeyPressed)
    {
        _recenterKeyPressed = true;
        _objectPosition.x = 0.0f;
        _objectPosition.y = 0.0f;
        _objectPosition.z = -6.0f;
    }
    else if (glfwGetKey(_window, GLFW_KEY_C) == GLFW_RELEASE)
        _recenterKeyPressed = false;

    if (_textureEnabled && _blendFactor < 1.0f)
        _blendFactor += 0.02f;
    if (!_textureEnabled && _blendFactor > 0.0f)
        _blendFactor -= 0.02f;

    if (_blendFactor < 0.0f)
        _blendFactor = 0.0f;
    if (_blendFactor > 1.0f)
        _blendFactor = 1.0f;
}

void App::run()
{
    while (!glfwWindowShouldClose(_window))
    {
        processInput();

        glClearColor(0.12f, 0.12f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = glfwGetTime();

        Mat4 rotation;
        if (_rotationAxis == 0)
            rotation = Mat4::rotationY(time);
        else if (_rotationAxis == 1)
            rotation = Mat4::rotationX(time);
        else
            rotation = Mat4::rotationZ(time);  

        Mat4 translation = Mat4::translation(
            _objectPosition.x,
            _objectPosition.y,
            _objectPosition.z
        );

        Mat4 model = rotation * translation;
        Mat4 view = Mat4::identity();

        float aspect = static_cast<float>(_width) / static_cast<float>(_height);
        Mat4 projection = Mat4::perspective(45.0f * 3.14159265f / 180.0f, aspect, 0.1f, 100.0f);

        _shader.use();

        GLuint modelLoc = glGetUniformLocation(_shader.getProgram(), "model");
        GLuint viewLoc = glGetUniformLocation(_shader.getProgram(), "view");
        GLuint projectionLoc = glGetUniformLocation(_shader.getProgram(), "projection");
        GLuint blendLoc = glGetUniformLocation(_shader.getProgram(), "uBlendFactor");
        GLuint textureLoc = glGetUniformLocation(_shader.getProgram(), "uTexture");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.m);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.m);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.m);
        glUniform1f(blendLoc, _blendFactor);
        glUniform1i(textureLoc, 0);

        _texture.bind(0);
        _mesh.draw();

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

void App::cleanup()
{
    _mesh.destroy();
    _texture.destroy();
    _shader.destroy();

    if (_window)
    {
        glfwDestroyWindow(_window);
        _window = NULL;
    }

    glfwTerminate();
}