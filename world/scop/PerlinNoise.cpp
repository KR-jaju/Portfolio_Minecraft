#include "pch.h"
#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
{
}

PerlinNoise::~PerlinNoise()
{
}

double PerlinNoise::getNoise2D
(
    double x, 
    double y, 
    int octav, 
    double delta_amplitude
) const
{
    double total = 0;
    double total_amplitude = 0;
    double frequency = 1;
    double amplitude = 1;
    for (int i = 0; i < octav; i++) {
        total += perlin2D(x * frequency, y * frequency) * amplitude;
        total_amplitude += amplitude;
        frequency *= 2;
        amplitude *= delta_amplitude;
    }
    return total / total_amplitude;
}

double PerlinNoise::getNoise3D(
    double x, 
    double y, 
    double z, 
    int octav, 
    double delta_amplitude
)
{
    double total = 0;
    double total_amplitude = 0;
    double frequency = 1;
    double amplitude = 1;
    for (int i = 0; i < octav; i++) {
        total += perlin(x * frequency, y * frequency, z * frequency) * amplitude;
        total_amplitude += amplitude;
        frequency *= 2;
        amplitude *= delta_amplitude;
    }
    return total / total_amplitude;
}

double PerlinNoise::fade(double t) const
{
    return t * t * t * (t * (6 * t - 15) + 10);
}

double PerlinNoise::lerp(double a, double b, double u) const
{
    return a * (1 - u) + b * u;
}

double PerlinNoise::perlin(double x, double y, double z) const
{
    int x0 = (static_cast<int>(floor(x))) & 255;
    int y0 = (static_cast<int>(floor(y))) & 255;
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    int a = this->permutation[x0] + y0;
    int b = this->permutation[x0 + 1] + y0;

    int aa = this->permutation[a];
    int ab = this->permutation[a + 1];
    int ba = this->permutation[b];
    int bb = this->permutation[b + 1];

    double a1 = lerp(gradiant(permutation[aa], x, y, z), 
        gradiant(permutation[ba], x - 1, y, z), u);
    double a2 = lerp(gradiant(permutation[ab], x, y - 1, z), 
        gradiant(permutation[bb], x - 1, y - 1, z), u);

    double b1 = lerp(gradiant(permutation[aa + 1], x, y, z - 1), 
        gradiant(permutation[ba + 1], x - 1, y, z - 1), u);
    double b2 = lerp(gradiant(permutation[ab + 1], x, y - 1, z - 1), 
        gradiant(permutation[bb + 1], x - 1, y - 1, z - 1), u);
    double lerp_1 = this->lerp(a1, a2, v);
    double lerp_2 = this->lerp(b1, b2, v);

    return this->lerp(lerp_1, lerp_2, w);
}

double PerlinNoise::gradiant(int hash, double x, double y, double z) const
{
    const std::uint8_t h = hash & 15;
    const double u = h < 8 ? x : y;
    const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

double PerlinNoise::perlin2D(double x, double y) const
{
    int xi = static_cast<int>(floor(x)) & 255;
    int yi = static_cast<int>(floor(y)) & 255;
    double xf = x - floor(x);
    double yf = y - floor(y);

    int a = this->permutation[xi] + yi;
    int b = this->permutation[xi + 1] + yi;
    int g1 = this->permutation[a];
    int g2 = this->permutation[b];
    int g3 = this->permutation[a + 1];
    int g4 = this->permutation[b + 1];

    double d1 = this->gradiant2D(g1, xf, yf);
    double d2 = this->gradiant2D(g2, xf - 1, yf);
    double d3 = this->gradiant2D(g3, xf, yf - 1);
    double d4 = this->gradiant2D(g4, xf - 1, yf - 1);

    double u = this->fade(xf);
    double v = this->fade(yf);

    double interpolation_x1 = this->lerp(d1, d2, u);
    double interpolation_x2 = this->lerp(d3, d4, u);
    return this->lerp(interpolation_x1, interpolation_x2, v);
}

double PerlinNoise::gradiant2D(int hash, double x, double y) const
{
    int d_flag = hash & 7;
    if (d_flag == 0)
        return x + y;
    if (d_flag == 1)
        return -x + y;
    if (d_flag == 2)
        return x - y;
    if (d_flag == 3)
        return -x - y;
    if (d_flag == 4)
        return y;
    if (d_flag == 5)
        return x;
    if (d_flag == 6)
        return -y;
    if (d_flag == 7)
        return -x;
    return 0;
}


