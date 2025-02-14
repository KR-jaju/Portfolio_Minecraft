#pragma once

struct IntVector2
{
    int x;
    int y;

    IntVector2()
        : x(0), y(0)
    {}

    IntVector2(int x, int y)
        : x(x), y(y)
    {}

    IntVector2 operator+(const IntVector2& other) const
    {
        return IntVector2(this->x + other.x, this->y + other.y);
    }

    IntVector2 operator-(const IntVector2& other) const
    {
        return IntVector2(this->x - other.x, this->y - other.y);
    }

    IntVector2 operator*(int scalar) const
    {
        return IntVector2(this->x * scalar, this->y * scalar);
    }

    IntVector2 operator/(int scalar) const
    {
        return IntVector2(this->x / scalar, this->y / scalar);
    }

    IntVector2& operator+=(const IntVector2& other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    IntVector2& operator-=(const IntVector2& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    IntVector2& operator*=(int scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        return *this;
    }

    IntVector2& operator/=(int scalar)
    {
        this->x /= scalar;
        this->y /= scalar;
        return *this;
    }

    bool operator==(const IntVector2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }

    bool operator!=(const IntVector2& other) const
    {
        return !(*this == other);
    }

    IntVector2 operator-() const
    {
        return IntVector2(-this->x, -this->y);
    }
};

namespace std
{
    template <>
    struct hash<IntVector2>
    {
        size_t operator()(IntVector2 p) const
        {
            size_t h1 = std::hash<int32_t>{}(p.x);
            size_t h2 = std::hash<int32_t>{}(p.y);

            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };
}
