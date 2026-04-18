#ifndef MAT4_HPP
#define MAT4_HPP

class Mat4
{
public:
    float m[16];

    Mat4();

    static Mat4 identity();
    static Mat4 translation(float x, float y, float z);
    static Mat4 rotationZ(float angle);
    static Mat4 perspective(float fovRadians, float aspect, float nearPlane, float farPlane);

    Mat4 operator*(const Mat4& other) const;
};

#endif