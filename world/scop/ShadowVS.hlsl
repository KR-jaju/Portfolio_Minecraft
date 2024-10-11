cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    int shadow_flag : SHADOW;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    int shadow_flag : SHADOW;
};


PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1);
    output.shadow_flag = input.shadow_flag;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    return output;
}