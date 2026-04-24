#include "../includes/Shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader() : _program(0)
{
}

Shader::~Shader()
{
    destroy();
}

std::string Shader::readFile(const std::string& path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return "";

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool Shader::compileShader(GLuint shader, const std::string& source, const std::string& name)
{
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::cerr << "Error: shader compilation failed (" << name << ")\n"
                  << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);

    if (vertexSource.empty())
    {
        std::cerr << "Error: could not read vertex shader: " << vertexPath << std::endl;
        return false;
    }

    if (fragmentSource.empty())
    {
        std::cerr << "Error: could not read fragment shader: " << fragmentPath << std::endl;
        return false;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    if (!compileShader(vertexShader, vertexSource, vertexPath))
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    if (!compileShader(fragmentShader, fragmentSource, fragmentPath))
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    _program = glCreateProgram();
    glAttachShader(_program, vertexShader);
    glAttachShader(_program, fragmentShader);
    glLinkProgram(_program);

    GLint success = 0;
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(_program, sizeof(infoLog), NULL, infoLog);
        std::cerr << "Error: shader program linking failed\n"
                  << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(_program);
        _program = 0;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

void Shader::use() const
{
    glUseProgram(_program);
}

void Shader::destroy()
{
    if (_program != 0)
    {
        glDeleteProgram(_program);
        _program = 0;
    }
}

GLuint Shader::getProgram() const
{
    return _program;
}