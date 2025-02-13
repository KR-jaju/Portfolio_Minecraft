
cbuffer CameraMatrices : register(b0)
{
    matrix view;
    matrix projection;
    matrix view_projection;
    matrix view_inverse_transpose;
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
    float3 normal : NORMAL;
    float3 uv : TEXCOORD;
};

float3 toNormal(uint direction)
{
    if (direction == 0) //east
        return float3(1, 0, 0);
    if (direction == 1)
        return float3(-1, 0, 0);
    if (direction == 2)
        return float3(0, 1, 0);
    if (direction == 3)
        return float3(0, -1, 0);
    if (direction == 4)
        return float3(0, 0, 1);
    return float3(0, 0, -1);
}

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    float4 ws_position = float4(input.pos + position * 16, 1.0);
    float4 vs_position = mul(ws_position, view);

    output.position = mul(vs_position, projection);
    output.uv = float3(input.uv, input.texture_id);
    output.normal = mul(toNormal(input.direction), view_inverse_transpose);
    return output;
}
