#ifndef OBJPARSER_HPP
#define OBJPARSER_HPP

#include <string>
#include <vector>
#include "Mesh.hpp"
#include "Vec3.hpp"
#include "Vec2.hpp"

struct Bounds
{
    float minX;
    float maxX;
    float minY;
    float maxY;
};

class ObjParser
{
public:
    static bool load(const std::string& path, std::vector<Vertex>& outVertices);

private:
    static bool parseVertexLine(const std::string& line, std::vector<Vec3>& positions);
    static bool parseTexCoordLine(const std::string& line, std::vector<Vec2>& texCoords);
    static bool parseFaceLine(const std::string& line,
                              const std::vector<Vec3>& positions,
                              const std::vector<Vec2>& texCoords,
                              const Bounds& bounds,
                              std::vector<Vertex>& outVertices);
    static bool parseFaceVertexToken(const std::string& token, int& positionIndex, int& texCoordIndex);
    static Bounds computeBounds(const std::vector<Vec3>& positions);
    static Vec2 generateUVFromPosition(const Vec3& p, const Bounds& bounds);
};

#endif