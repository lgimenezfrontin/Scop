#include "../includes/Mesh.hpp"

Vertex::Vertex() : position()
{
}

Vertex::Vertex(const Vec3& position) : position(position)
{
}

Mesh::Mesh() : _vao(0), _vbo(0)
{
}

Mesh::~Mesh()
{
    destroy();
}

bool Mesh::upload(const std::vector<Vertex>& vertices)
{
    _vertices = vertices;

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW); // ajouter garde-fou pour &_vertice[0]

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

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
}