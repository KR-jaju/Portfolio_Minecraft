
cbuffer ShadowCaster : register(b0)
{
    matrix shadow_caster_view_projection;
};

cbuffer WorldMatrix : register(b1)
{
    matrix world;
};


struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    uint texture_id : BLENDINDICES;
    uint direction : TYPE;
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

float4 main(VS_INPUT input) : SV_Position
{
    float4 os_position = float4(input.pos, 1);
    float4 ws_position = mul(os_position, world);
    float4 vs_position = mul(ws_position, shadow_caster_view_projection[0]);

    return (vs_position);
}
