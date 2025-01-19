
struct PSInput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
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
    output.uv = texcoords[vertexID];
    return output;
}
