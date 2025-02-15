
cbuffer CameraMatrices : register(b0)
{
    matrix view;
    matrix projection;
    matrix view_projection;
    matrix view_inverse_transpose;
    matrix projection_inverse;
    int2   dimension;
};

struct PSInput {
    float4 position : SV_POSITION;
    float2 scaled_uv : TEXCOORD0;
};

PSInput main(uint vertexID : SV_VertexID) {
    PSInput output;
    float2 positions[4] = {
        float2(-1.0, -1.0),
        float2(-1.0,  1.0),
        float2(1.0, -1.0),
        float2(1.0,  1.0)
    };
    float2 texcoords[4] = {
        float2(0.0, 1.0), // UV ÁÂÇ¥
        float2(0.0, 0.0),
        float2(1.0, 1.0),
        float2(1.0, 0.0)
    };

    output.position = float4(positions[vertexID], 0.0, 1.0);
    output.scaled_uv = texcoords[vertexID] * dimension;
    return output;
}
