#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <string>

class Texture
{
public:
    Texture();
    ~Texture();

    bool loadBMP(const std::string& path);
    void bind(GLuint unit = 0) const;
    void destroy();

private:
    GLuint _id;
};

#endif