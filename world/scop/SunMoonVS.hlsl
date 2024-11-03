struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

cbuffer mvp : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
}

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.pos = mul(output.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    output.color = input.color;
    return output;
}