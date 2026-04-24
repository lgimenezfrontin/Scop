#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <vector>
#include "Vec3.hpp"
#include "Vec2.hpp"

struct Vertex
{
    Vec3 position;
    Vec3 color;
    Vec2 uv;

    Vertex();
    Vertex(const Vec3& position, const Vec3& color, const Vec2& uv);
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    bool upload(const std::vector<Vertex>& vertices);
    void draw() const;
    void destroy();

    Vec3 getCenter() const;

private:
    std::vector<Vertex> _vertices;
    GLuint _vao;
    GLuint _vbo;
    Vec3 _center;

    void computeCenter();
    void recenterVertices();
};

#endif