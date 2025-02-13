#pragma once

struct IntVector3
{
    int x;
    int y;
    int z;

    IntVector3()
        : x(0), y(0), z(0)
    {}

    IntVector3(int x, int y, int z)
        : x(x), y(y), z(z)
    {}

    IntVector3 operator+(const IntVector3& other) const
    {
        return IntVector3(this->x + other.x, this->y + other.y, this->z + other.z);
    }

    IntVector3 operator-(const IntVector3& other) const
    {
        return IntVector3(this->x - other.x, this->y - other.y, this->z - other.z);
    }

    IntVector3 operator*(int scalar) const
    {
        return IntVector3(this->x * scalar, this->y * scalar, this->z * scalar);
    }

    IntVector3 operator/(int scalar) const
    {
        return IntVector3(this->x / scalar, this->y / scalar, this->z / scalar);
    }

    IntVector3& operator+=(const IntVector3& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    IntVector3& operator-=(const IntVector3& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }

    IntVector3& operator*=(int scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        return *this;
    }

    IntVector3& operator/=(int scalar)
    {
        this->x /= scalar;
        this->y /= scalar;
        this->z /= scalar;
        return *this;
    }

    bool operator==(const IntVector3& other) const
    {
        return this->x == other.x && this->y == other.y && this->z == other.z;
    }

    bool operator!=(const IntVector3& other) const
    {
        return !(*this == other);
    }

    IntVector3 operator-() const
    {
        return IntVector3(-this->x, -this->y, -this->z);
    }
};

namespace std
{
    template <>
    struct hash<IntVector3>
    {
        size_t operator()(const IntVector3& p) const
        {
            size_t h1 = std::hash<int32_t>{}(p.x);
            size_t h2 = std::hash<int32_t>{}(p.y);
            size_t h3 = std::hash<int32_t>{}(p.z);

            size_t seed = h1;
            seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}
