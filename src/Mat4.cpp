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
    result.m[0] = 1;
    result.m[5] = 1;
    result.m[10] = 1;
    result.m[15] = 1;
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