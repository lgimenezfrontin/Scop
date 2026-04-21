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

bool ObjParser::load(const std::string& path, std::vector<Vertex>& outVertices)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open OBJ file: " << path << std::endl;
        return false;
    }

    std::vector<Vec3> positions;
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
        else if (line.size() >= 2 && line[0] == 'f' && line[1] == ' ')
        {
            if (!parseFaceLine(line, positions, outVertices))
                return false;
        }
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

bool ObjParser::parseFaceVertexToken(const std::string& token, int& positionIndex)
{
    if (token.empty())
        return false;

    std::string positionPart;
    std::size_t slashPos = token.find('/');

    if (slashPos == std::string::npos)
        positionPart = token;
    else
        positionPart = token.substr(0, slashPos);

    if (positionPart.empty())
        return false;

    positionIndex = std::atoi(positionPart.c_str());
    if (positionIndex <= 0)
        return false;

    return true;
}

bool ObjParser::parseFaceLine(const std::string& line,
                              const std::vector<Vec3>& positions,
                              std::vector<Vertex>& outVertices)
{
    std::istringstream iss(line);
    std::string type;
    std::string token;
    std::vector<int> faceIndices;

    iss >> type;
    if (iss.fail())
    {
        std::cerr << "Error: invalid face line: " << line << std::endl;
        return false;
    }

    while (iss >> token)
    {
        int index;
        if (!parseFaceVertexToken(token, index))
        {
            std::cerr << "Error: invalid face vertex token in line: " << line << std::endl;
            return false;
        }

        if (index <= 0 || index > static_cast<int>(positions.size()))
        {
            std::cerr << "Error: face index out of range: " << line << std::endl;
            return false;
        }

        faceIndices.push_back(index);
    }

    if (faceIndices.size() < 3)
    {
        std::cerr << "Error: face has fewer than 3 vertices: " << line << std::endl;
        return false;
    }
        
    if (faceIndices.size() == 3)
    {
        Vec3 color = colors[colorIndex % 6];
        colorIndex++;

        outVertices.push_back(Vertex(positions[faceIndices[0] - 1], color));
        outVertices.push_back(Vertex(positions[faceIndices[1] - 1], color));
        outVertices.push_back(Vertex(positions[faceIndices[2] - 1], color));

        return true;
    }

    if (faceIndices.size() == 4)
    {
        Vec3 color = colors[colorIndex % 6];
        colorIndex++;

        // triangle 1
        outVertices.push_back(Vertex(positions[faceIndices[0] - 1], color));
        outVertices.push_back(Vertex(positions[faceIndices[1] - 1], color));
        outVertices.push_back(Vertex(positions[faceIndices[2] - 1], color));

        // triangle 2
        outVertices.push_back(Vertex(positions[faceIndices[0] - 1], color));
        outVertices.push_back(Vertex(positions[faceIndices[2] - 1], color));
        outVertices.push_back(Vertex(positions[faceIndices[3] - 1], color));

        return true;
    }

    std::cerr << "Error: only triangles and quads are supported: " << line << std::endl;
    return false;
}