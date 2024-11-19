struct VS_INPUT
{
    int tex_arr_idx : INDEX;
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct HS_INPUT
{
    int tex_arr_idx : INDEX;
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world_pos : POSITION_WORLD;
    float2 uv : TEXCOORD;
};


HS_INPUT main(VS_INPUT input)
{
    HS_INPUT output;
    output.pos = float4(input.pos, 1);
    output.world_pos = input.pos;
    output.normal = input.normal;
    output.uv = input.uv;
    output.tangent = input.tangent;
    output.tex_arr_idx = input.tex_arr_idx;
    return output;
}