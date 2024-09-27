cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

struct VS_INPUT
{
    int type : TYPE;
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    int shadow_flag : SHADOW;
};

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    int shadow_flag : SHADOW;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    
    float4 pos = float4(input.pos, 1);
    output.type = input.type;
    output.pos = pos;
    output.world_pos = input.pos;
    output.uv = input.uv;
    output.dir = input.dir;
    output.normal = input.normal;
    output.shadow_flag = input.shadow_flag;
    //output.x_pos = input.x_pos;
    
    output.pos = mul(output.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    //pos = mul(pos, l_view);
    //pos = mul(pos, l_proj);
    //pos.xyz /= pos.w;
    return output;
}