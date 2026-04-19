#ifndef OBJPARSER_HPP
#define OBJPARSER_HPP

#include <string>
#include <vector>
#include "Mesh.hpp"
#include "Vec3.hpp"

class ObjParser
{
public:
    static bool load(const std::string& path, std::vector<Vertex>& outVertices);

private:
    static bool parseVertexLine(const std::string& line, std::vector<Vec3>& positions);
    static bool parseFaceLine(const std::string& line,
                              const std::vector<Vec3>& positions,
                              std::vector<Vertex>& outVertices);
    static bool parseFaceVertexToken(const std::string& token, int& positionIndex);
};

#endif