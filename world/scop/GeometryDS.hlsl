cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

cbuffer ConstData : register(b1)
{
    float3 eye_pos;
}

struct DS_INPUT
{
    int tex_arr_idx : INDEX;
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world_pos : POSITION_WORLD;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    int tex_arr_idx : INDEX;
    float4 pos : SV_Position;
    float3 w_pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct PatchConstOutput
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 4

[domain("quad")]
PS_INPUT main(
	PatchConstOutput input,
	float2 uv : SV_DomainLocation,
	const OutputPatch<DS_INPUT, NUM_CONTROL_POINTS> patch)
{
	PS_INPUT output;

    float3 position;
    float3 v1 = lerp(patch[0].pos.xyz, patch[1].pos.xyz, uv.x);
    float3 v2 = lerp(patch[2].pos.xyz, patch[3].pos.xyz, uv.x);
    position = lerp(v1, v2, uv.y);
    output.pos = float4(position, 1.0);
    output.w_pos = position;
    output.tex_arr_idx = patch[0].tex_arr_idx;
    float2 uv1 = lerp(patch[0].uv, patch[1].uv, uv.x);
    float2 uv2 = lerp(patch[2].uv, patch[3].uv, uv.x);
    output.uv = lerp(uv1, uv2, uv.y);
    
    float3 center = float3(0, 0, 0);
    for (int i = 0; i < 4; i++)
        center += patch[i].pos.xyz;
    center *= 0.25;
    float dist = length(center - eye_pos);
    float dist_min = 0.5;
    float dist_max = 20;
    float lod = 1.0f - 
        saturate((dist_max - dist) / (dist_max - dist_min));
    lod *= 8;
    //output.lod = 0;
    
    output.normal = patch[0].normal;
    output.tangent = patch[0].tangent;
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    return output;
}
