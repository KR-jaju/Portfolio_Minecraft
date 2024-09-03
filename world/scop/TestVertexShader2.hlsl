cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

struct VS_INPUT
{
    int type : TYPE;
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    int2 chunk_idx : CHUNKIDX;
};

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    int2 chunk_idx : CHUNKIDX;
};

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    output.type = input.type;
    output.pos = float4(input.pos, 1);
    output.world_pos = input.pos;
    output.uv = input.uv;
    output.dir = input.dir;
    output.chunk_idx = input.chunk_idx;
    
    output.pos = mul(output.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    return output;
}