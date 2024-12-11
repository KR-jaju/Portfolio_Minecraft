cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    int shadow_flag : SHADOW;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 w_pos : POSITION;
    float3 normal : NORMAL;
    float eye_z : VIEW_SPACE_Z;
    int shadow_flag : SHADOW;
};


PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1);
    output.w_pos = output.pos;
    output.shadow_flag = input.shadow_flag;
    
    output.pos = mul(output.pos, view);
    output.eye_z = output.pos.z;
    output.pos = mul(output.pos, proj);
    output.normal = input.normal;
    return output;
}