cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 w_pos : POSITION;
};


PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.w_pos = float4(input.pos, 1);
    output.pos = mul(output.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    return output;
}