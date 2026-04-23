#include "../includes/Mat4.hpp"
#include <cmath>

Mat4::Mat4()
{
    for (int i = 0; i < 16; i++)
        m[i] = 0.0f;
}

Mat4 Mat4::identity()
{
    Mat4 result;
    result.m[0] = 1.0f;
    result.m[5] = 1.0f;
    result.m[10] = 1.0f;
    result.m[15] = 1.0f;
    return result;
}

Mat4 Mat4::translation(float x, float y, float z)
{
    Mat4 result = identity();
    result.m[12] = x;
    result.m[13] = y;
    result.m[14] = z;
    return result;
}

Mat4 Mat4::rotationX(float angle)
{
    Mat4 result = identity();

    float c = std::cos(angle);
    float s = std::sin(angle);

    result.m[5] = c;
    result.m[6] = s;
    result.m[9] = -s;
    result.m[10] = c;

    return result;
}

Mat4 Mat4::rotationY(float angle)
{
    Mat4 result = identity();

    float c = std::cos(angle);
    float s = std::sin(angle);

    result.m[0] = c;
    result.m[2] = -s;
    result.m[8] = s;
    result.m[10] = c;

    return result;
}

Mat4 Mat4::rotationZ(float angle)
{
    Mat4 result = identity();

    float c = cos(angle);
    float s = sin(angle);

    result.m[0] = c;
    result.m[1] = s;
    result.m[4] = -s;
    result.m[5] = c;

    return result;
}

Mat4 Mat4::perspective(float fovRadians, float aspect, float nearPlane, float farPlane)
{
    Mat4 result;

    float tanHalfFov = std::tan(fovRadians / 2.0f);

    result.m[0] = 1.0f / (aspect * tanHalfFov);
    result.m[5] = 1.0f / tanHalfFov;
    result.m[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    result.m[11] = -1.0f;
    result.m[14] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

    return result;
}

Mat4 Mat4::operator*(const Mat4& other) const
{
    Mat4 result;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            result.m[col + row * 4] =
                m[0 + row * 4] * other.m[col + 0 * 4] +
                m[1 + row * 4] * other.m[col + 1 * 4] +
                m[2 + row * 4] * other.m[col + 2 * 4] +
                m[3 + row * 4] * other.m[col + 3 * 4];
        }
    }

    return result;
}