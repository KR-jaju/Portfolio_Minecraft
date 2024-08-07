#include "pch.h"
#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
{
}

PerlinNoise::~PerlinNoise()
{
}

float PerlinNoise::getNoise2D
(
    float x, 
    float y, 
    int octav, 
    float delta_amplitude
) const
{
    float total = 0;
    float total_amplitude = 0;
    float frequency = 1;
    float amplitude = 1;
    for (int i = 0; i < octav; i++) {
        total += perlin(x * frequency, y * frequency, 0) * amplitude;
        total_amplitude += amplitude;
        frequency *= 2;
        amplitude *= delta_amplitude;
    }
    return total / total_amplitude;
}

float PerlinNoise::fade(float t) const
{
    return t * t * t * (t * (6 * t - 15) + 10);
}

float PerlinNoise::lerp(float a, float b, float u) const
{
    return a * (1 - u) + b * u;
}

float PerlinNoise::perlin(float x, float y, float z) const
{
    int x0 = (static_cast<int>(floor(x))) & 255;
    int y0 = (static_cast<int>(floor(y))) & 255;
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    int a = this->permutation[x0] + y0;
    int b = this->permutation[x0 + 1] + y0;

    int aa = this->permutation[a];
    int ab = this->permutation[a + 1];
    int ba = this->permutation[b];
    int bb = this->permutation[b + 1];

    float a1 = lerp(gradiant(permutation[aa], x, y, z), 
        gradiant(permutation[ba], x - 1, y, z), u);
    float a2 = lerp(gradiant(permutation[ab], x, y - 1, z), 
        gradiant(permutation[bb], x - 1, y - 1, z), u);

    float b1 = lerp(gradiant(permutation[aa + 1], x, y, z - 1), 
        gradiant(permutation[ba + 1], x - 1, y, z - 1), u);
    float b2 = lerp(gradiant(permutation[ab + 1], x, y - 1, z - 1), 
        gradiant(permutation[bb + 1], x - 1, y - 1, z - 1), u);
    float lerp_1 = this->lerp(a1, a2, v);
    float lerp_2 = this->lerp(b1, b2, v);

    return this->lerp(lerp_1, lerp_2, w);
}

float PerlinNoise::gradiant(int hash, float x, float y, float z) const
{
    const std::uint8_t h = hash & 15;
    const float u = h < 8 ? x : y;
    const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}


