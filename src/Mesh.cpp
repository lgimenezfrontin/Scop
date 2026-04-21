#include "../includes/Mesh.hpp"

Vertex::Vertex() : position(), color()
{
}

Vertex::Vertex(const Vec3& position, const Vec3& color)
    : position(position), color(color)
{
}

Mesh::Mesh() : _vao(0), _vbo(0), _center()
{
}

Mesh::~Mesh()
{
    destroy();
}

void Mesh::computeCenter()
{
    if (_vertices.empty())
    {
        _center = Vec3(0.0f, 0.0f, 0.0f);
        return;
    }

    float minX = _vertices[0].position.x;
    float maxX = _vertices[0].position.x;
    float minY = _vertices[0].position.y;
    float maxY = _vertices[0].position.y;
    float minZ = _vertices[0].position.z;
    float maxZ = _vertices[0].position.z;

    for (std::size_t i = 1; i < _vertices.size(); i++)
    {
        const Vec3& p = _vertices[i].position;

        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
        if (p.z < minZ) minZ = p.z;
        if (p.z > maxZ) maxZ = p.z;
    }

    _center = Vec3(
        (minX + maxX) * 0.5f,
        (minY + maxY) * 0.5f,
        (minZ + maxZ) * 0.5f
    );
}

void Mesh::recenterVertices()
{
    for (std::size_t i = 0; i < _vertices.size(); i++)
        _vertices[i].position = _vertices[i].position - _center;
}

bool Mesh::upload(const std::vector<Vertex>& vertices)
{
    if (vertices.empty())
        return false;

    _vertices = vertices;
    computeCenter();
    recenterVertices();

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 _vertices.size() * sizeof(Vertex),
                 &_vertices[0],
                 GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // couleur
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void Mesh::draw() const
{
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    glBindVertexArray(0);
}

Vec3 Mesh::getCenter() const
{
    return _center;
}

void Mesh::destroy()
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

    _vertices.clear();
    _center = Vec3(0.0f, 0.0f, 0.0f);
}