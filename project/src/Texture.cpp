#include "../includes/Texture.hpp"
#include <fstream>
#include <iostream>
#include <vector>

Texture::Texture() : _id(0)
{
}

Texture::~Texture()
{
    destroy();
}

static int readIntLE(const unsigned char* data)
{
    return static_cast<int>(data[0]) |
           (static_cast<int>(data[1]) << 8) |
           (static_cast<int>(data[2]) << 16) |
           (static_cast<int>(data[3]) << 24);
}

static short readShortLE(const unsigned char* data)
{
    return static_cast<short>(
        static_cast<int>(data[0]) |
        (static_cast<int>(data[1]) << 8)
    );
}

bool Texture::loadBMP(const std::string& path)
{
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: could not open texture file: " << path << std::endl;
        return false;
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);
    if (file.gcount() != 54)
    {
        std::cerr << "Error: invalid BMP header: " << path << std::endl;
        return false;
    }

    if (header[0] != 'B' || header[1] != 'M')
    {
        std::cerr << "Error: file is not a BMP: " << path << std::endl;
        return false;
    }

    int dataOffset = readIntLE(&header[10]);
    int width = readIntLE(&header[18]);
    int height = readIntLE(&header[22]);
    short bitsPerPixel = readShortLE(&header[28]);
    int imageSize = readIntLE(&header[34]);

    if (bitsPerPixel != 24)
    {
        std::cerr << "Error: only 24-bit BMP is supported: " << path << std::endl;
        return false;
    }

    if (imageSize == 0)
        imageSize = width * height * 3;

    std::vector<unsigned char> data(imageSize);

    file.seekg(dataOffset, std::ios::beg);
    file.read(reinterpret_cast<char*>(&data[0]), imageSize);
    if (!file)
    {
        std::cerr << "Error: failed to read BMP pixel data: " << path << std::endl;
        return false;
    }

    for (int i = 0; i < imageSize; i += 3)
    {
        unsigned char tmp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = tmp;
    }

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void Texture::bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::destroy()
{
    if (_id != 0)
    {
        glDeleteTextures(1, &_id);
        _id = 0;
    }
}