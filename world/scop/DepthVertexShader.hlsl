cbuffer ConstantBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
}

struct VS_INPUT
{
    int type : TYPE;
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    //int x_pos : XPOS;
};

struct PS_INPUT
{
    float4 ndc_pos : SV_Position;
    float4 clip_pos : POSITION;
};

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    output.ndc_pos = float4(input.pos, 1);
    output.ndc_pos = mul(output.ndc_pos, view);
    output.ndc_pos = mul(output.ndc_pos, proj);
    output.clip_pos = output.ndc_pos;
    return output;
}