#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <string>

class Shader
{
public:
    Shader();
    ~Shader();

    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const;
    void destroy();

    GLuint getProgram() const;

private:
    GLuint _program;

    std::string readFile(const std::string& path);
    bool compileShader(GLuint shader, const std::string& source, const std::string& name);
};

#endif