#include "pch.h"
#include "World.h"

World::World()
{
}

World::~World()
{
}

void World::MakeBox() {

    vector<vec3> positions;
    vector<vec3> colors;
    vector<vec3> normals;

    const float scale = 1.0f;

    // 윗면
    positions.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
    colors.push_back(vec3(1.0f, 0.0f, 0.0f));
    colors.push_back(vec3(1.0f, 0.0f, 0.0f));
    colors.push_back(vec3(1.0f, 0.0f, 0.0f));
    colors.push_back(vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(vec3(0.0f, 1.0f, 0.0f));

    // 아랫면
    positions.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
    colors.push_back(vec3(0.0f, 1.0f, 0.0f));
    colors.push_back(vec3(0.0f, 1.0f, 0.0f));
    colors.push_back(vec3(0.0f, 1.0f, 0.0f));
    colors.push_back(vec3(0.0f, 1.0f, 0.0f));
    normals.push_back(vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(vec3(0.0f, -1.0f, 0.0f));
    normals.push_back(vec3(0.0f, -1.0f, 0.0f));

    // 앞면
    positions.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(vec3(0.0f, 0.0f, 1.0f));
    colors.push_back(vec3(0.0f, 0.0f, 1.0f));
    colors.push_back(vec3(0.0f, 0.0f, 1.0f));
    colors.push_back(vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(vec3(0.0f, 0.0f, -1.0f));
    normals.push_back(vec3(0.0f, 0.0f, -1.0f));

    // 뒷면
    positions.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(vec3(0.0f, 1.0f, 1.0f));
    colors.push_back(vec3(0.0f, 1.0f, 1.0f));
    colors.push_back(vec3(0.0f, 1.0f, 1.0f));
    colors.push_back(vec3(0.0f, 1.0f, 1.0f));
    normals.push_back(vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(vec3(0.0f, 0.0f, 1.0f));
    normals.push_back(vec3(0.0f, 0.0f, 1.0f));

    // 왼쪽
    positions.push_back(vec3(-1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(vec3(-1.0f, 1.0f, 1.0f) * scale);
    positions.push_back(vec3(-1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(vec3(-1.0f, -1.0f, -1.0f) * scale);
    colors.push_back(vec3(1.0f, 1.0f, 0.0f));
    colors.push_back(vec3(1.0f, 1.0f, 0.0f));
    colors.push_back(vec3(1.0f, 1.0f, 0.0f));
    colors.push_back(vec3(1.0f, 1.0f, 0.0f));
    normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(-1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(-1.0f, 0.0f, 0.0f));

    // 오른쪽
    positions.push_back(vec3(1.0f, -1.0f, 1.0f) * scale);
    positions.push_back(vec3(1.0f, -1.0f, -1.0f) * scale);
    positions.push_back(vec3(1.0f, 1.0f, -1.0f) * scale);
    positions.push_back(vec3(1.0f, 1.0f, 1.0f) * scale);
    colors.push_back(vec3(1.0f, 0.0f, 1.0f));
    colors.push_back(vec3(1.0f, 0.0f, 1.0f));
    colors.push_back(vec3(1.0f, 0.0f, 1.0f));
    colors.push_back(vec3(1.0f, 0.0f, 1.0f));
    normals.push_back(vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(1.0f, 0.0f, 0.0f));
    normals.push_back(vec3(1.0f, 0.0f, 0.0f));

    vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.pos = positions[i];
        v.color = colors[i];
        vertices.push_back(v);
    }

    vector<uint16_t> indices = {
        0,  1,  2,  0,  2,  3,  // 윗면
        4,  5,  6,  4,  6,  7,  // 아랫면
        8,  9,  10, 8,  10, 11, // 앞면
        12, 13, 14, 12, 14, 15, // 뒷면
        16, 17, 18, 16, 18, 19, // 왼쪽
        20, 21, 22, 20, 22, 23  // 오른쪽
    };
}
