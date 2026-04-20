#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <vector>
#include "Vec3.hpp"

struct Vertex
{
    Vec3 position;

    Vertex();
    Vertex(const Vec3& position);
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