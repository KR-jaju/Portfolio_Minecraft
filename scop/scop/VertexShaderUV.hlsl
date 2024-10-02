cbuffer CameraData : register(b0)
{
    matrix view;
    matrix proj;
}

cbuffer TransformData : register(b1)
{
    row_major matrix matWorld;
}

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1);
    output.uv = input.uv;
    
    output.pos = mul(output.pos, matWorld);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    return output;
}

Texture2D texture0 : register(t0);