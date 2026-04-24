#include "../includes/ObjParser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

static const Vec3 colors[] = {
    Vec3(1.0f, 0.0f, 0.0f),
    Vec3(0.0f, 1.0f, 0.0f),
    Vec3(0.0f, 0.0f, 1.0f),
    Vec3(1.0f, 1.0f, 0.0f),
    Vec3(1.0f, 0.0f, 1.0f),
    Vec3(0.0f, 1.0f, 1.0f)
};

static int colorIndex = 0;

Bounds ObjParser::computeBounds(const std::vector<Vec3>& positions)
{
    Bounds bounds;

    bounds.minX = positions[0].x;
    bounds.maxX = positions[0].x;
    bounds.minY = positions[0].y;
    bounds.maxY = positions[0].y;

    for (std::size_t i = 1; i < positions.size(); i++)
    {
        if (positions[i].x < bounds.minX)
            bounds.minX = positions[i].x;
        if (positions[i].x > bounds.maxX)
            bounds.maxX = positions[i].x;
        if (positions[i].y < bounds.minY)
            bounds.minY = positions[i].y;
        if (positions[i].y > bounds.maxY)
            bounds.maxY = positions[i].y;
    }

    return bounds;
}

Vec2 ObjParser::generateUVFromPosition(const Vec3& p, const Bounds& bounds)
{
    float u = 0.0f;
    float v = 0.0f;

    if (bounds.maxX > bounds.minX)
        u = (p.x - bounds.minX) / (bounds.maxX - bounds.minX);

    if (bounds.maxY > bounds.minY)
        v = (p.y - bounds.minY) / (bounds.maxY - bounds.minY);

    return Vec2(u, v);
}

bool ObjParser::load(const std::string& path, std::vector<Vertex>& outVertices)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open OBJ file: " << path << std::endl;
        return false;
    }

    std::vector<Vec3> positions;
    std::vector<Vec2> texCoords;
    std::vector<std::string> faceLines;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;

        if (line.size() >= 2 && line[0] == 'v' && line[1] == ' ')
        {
            if (!parseVertexLine(line, positions))
                return false;
        }
        else if (line.size() >= 3 && line[0] == 'v' && line[1] == 't' && line[2] == ' ')
        {
            if (!parseTexCoordLine(line, texCoords))
                return false;
        }
        else if (line.size() >= 2 && line[0] == 'f' && line[1] == ' ')
        {
            faceLines.push_back(line);
        }
    }

    if (positions.empty())
    {
        std::cerr << "Error: OBJ contains no positions: " << path << std::endl;
        return false;
    }

    Bounds bounds = computeBounds(positions);

    for (std::size_t i = 0; i < faceLines.size(); i++)
    {
        if (!parseFaceLine(faceLines[i], positions, texCoords, bounds, outVertices))
            return false;
    }

    if (outVertices.empty())
    {
        std::cerr << "Error: OBJ produced no vertices: " << path << std::endl;
        return false;
    }

    return true;
}

bool ObjParser::parseVertexLine(const std::string& line, std::vector<Vec3>& positions)
{
    std::istringstream iss(line);
    char type;
    float x, y, z;

    iss >> type >> x >> y >> z;
    if (iss.fail())
    {
        std::cerr << "Error: invalid vertex line: " << line << std::endl;
        return false;
    }

    positions.push_back(Vec3(x, y, z));
    return true;
}

bool ObjParser::parseTexCoordLine(const std::string& line, std::vector<Vec2>& texCoords)
{
    std::istringstream iss(line);
    std::string type;
    float u, v;

    iss >> type >> u >> v;
    if (iss.fail())
    {
        std::cerr << "Error: invalid texture coordinate line: " << line << std::endl;
        return false;
    }

    texCoords.push_back(Vec2(u, v));
    return true;
}

bool ObjParser::parseFaceVertexToken(const std::string& token, int& positionIndex, int& texCoordIndex)
{
    positionIndex = 0;
    texCoordIndex = 0;

    std::size_t firstSlash = token.find('/');
    if (firstSlash == std::string::npos)
    {
        positionIndex = std::atoi(token.c_str());
        return (positionIndex > 0);
    }

    std::string positionPart = token.substr(0, firstSlash);
    if (positionPart.empty())
        return false;

    positionIndex = std::atoi(positionPart.c_str());
    if (positionIndex <= 0)
        return false;

    std::size_t secondSlash = token.find('/', firstSlash + 1);

    if (secondSlash == std::string::npos)
    {
        std::string texPart = token.substr(firstSlash + 1);
        if (!texPart.empty())
            texCoordIndex = std::atoi(texPart.c_str());
    }
    else
    {
        std::string texPart = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
        if (!texPart.empty())
            texCoordIndex = std::atoi(texPart.c_str());
    }

    return true;
}

bool ObjParser::parseFaceLine(const std::string& line,
                              const std::vector<Vec3>& positions,
                              const std::vector<Vec2>& texCoords,
                              const Bounds& bounds,
                              std::vector<Vertex>& outVertices)
{
    std::istringstream iss(line);
    std::string type;
    std::string token;

    std::vector<int> positionIndices;
    std::vector<int> texCoordIndices;

    iss >> type;
    if (iss.fail())
    {
        std::cerr << "Error: invalid face line: " << line << std::endl;
        return false;
    }

    while (iss >> token)
    {
        int positionIndex;
        int texCoordIndex;

        if (!parseFaceVertexToken(token, positionIndex, texCoordIndex))
        {
            std::cerr << "Error: invalid face vertex token in line: " << line << std::endl;
            return false;
        }

        if (positionIndex <= 0 || positionIndex > static_cast<int>(positions.size()))
        {
            std::cerr << "Error: face position index out of range: " << line << std::endl;
            return false;
        }

        if (texCoordIndex < 0 || texCoordIndex > static_cast<int>(texCoords.size()))
        {
            std::cerr << "Error: face texture index out of range: " << line << std::endl;
            return false;
        }

        positionIndices.push_back(positionIndex);
        texCoordIndices.push_back(texCoordIndex);
    }

    if (positionIndices.size() < 3)
    {
        std::cerr << "Error: face has fewer than 3 vertices: " << line << std::endl;
        return false;
    }

    Vec3 color = colors[colorIndex % 6];
    colorIndex++;

    if (positionIndices.size() == 3)
    {
        for (int i = 0; i < 3; i++)
        {
            Vec3 pos = positions[positionIndices[i] - 1];
            Vec2 uv;

            if (texCoordIndices[i] > 0)
                uv = texCoords[texCoordIndices[i] - 1];
            else
                uv = generateUVFromPosition(pos, bounds);

            outVertices.push_back(Vertex(pos, color, uv));
        }
        return true;
    }

    if (positionIndices.size() == 4)
    {
        int triIndices[6] = {0, 1, 2, 0, 2, 3};

        for (int i = 0; i < 6; i++)
        {
            int idx = triIndices[i];
            Vec3 pos = positions[positionIndices[idx] - 1];
            Vec2 uv;

            if (texCoordIndices[idx] > 0)
                uv = texCoords[texCoordIndices[idx] - 1];
            else
                uv = generateUVFromPosition(pos, bounds);

            outVertices.push_back(Vertex(pos, color, uv));
        }
        return true;
    }

    std::cerr << "Error: only triangles and quads are supported: " << line << std::endl;
    return false;
}