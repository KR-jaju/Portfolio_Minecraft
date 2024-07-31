cbuffer MVP : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
}

struct VS_INPUT
{
    int type : TYPE;
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT
{
    int type : Type;
    float4 pos : SV_Position;
    float4 color : COLOR;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1.f);
    output.color = input.color;
    
    output.pos = mul(output.pos, model);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    return output;
}