
cbuffer CameraMatrices : register(b0)
{
    matrix view;
    matrix projection;
    matrix view_projection;
    matrix view_inverse;
    matrix projection_inverse;
    int2   dimension;
};

cbuffer WorldMatrix : register(b1)
{
    int3 position;
};


struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    uint texture_id : BLENDINDICES;
    uint direction : TYPE;
    //int type : TYPE;
    //float3 pos : POSITION;
    //float3 normal : NORMAL;
    //float2 uv : TEXCOORD;
    //int dir : DIRECTION;
};

struct PS_INPUT
{
    float4 position : SV_Position;
};

float4 main(VS_INPUT input) : SV_Position
{
    float4 ws_position = float4(input.pos + position * 16, 1.0);
    float4 vs_position = mul(ws_position, view);

    return mul(vs_position, projection);
}
