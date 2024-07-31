struct VS_INPUT
{
    float3 pos : POSITION;
    float4 col : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float4 col : COLOR;
};

cbuffer MVPMat : register(b0)
{
    matrix model;
    matrix view;
    matrix proj;
};

VS_OUTPUT main( VS_INPUT input )
{
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1.f);
    output.col = input.col;
    
    output.pos = mul(output.pos, model);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    return output;
}