struct HS_INPUT
{
    int tex_arr_idx : INDEX;
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world_pos : POSITION_WORLD;
    float2 uv : TEXCOORD;
};

struct DS_INPUT
{
    int tex_arr_idx : INDEX;
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world_pos : POSITION_WORLD;
    float2 uv : TEXCOORD;
};

cbuffer ConstData : register(b0)
{
    float3 eye_pos;
}

struct PatchConstOutput
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 4

float calculateTess(
    InputPatch<HS_INPUT, NUM_CONTROL_POINTS> ip
)
{
    float3 center = float3(0, 0, 0);
    for (int i = 0; i < 4; i++)
        center += ip[i].pos.xyz;
    center *= 0.25;
    float dist = length(center - eye_pos);
    float dist_min = 0.5;
    float dist_max = 20;
    float tess = 64 *
        saturate((dist_max - dist) / (dist_max - dist_min)) + 1;
    return tess;
}

PatchConstOutput CalcHSPatchConstants(
	InputPatch<HS_INPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	
    PatchConstOutput output;
    float tess;
    //tess = calculateTess(ip); // 동적으로 정점 수 조절
    tess = 1;
    output.edges[0] = tess;
    output.edges[1] = tess;
    output.edges[2] = tess;
    output.edges[3] = tess;
    output.inside[0] = tess;
    output.inside[1] = tess;
    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CalcHSPatchConstants")]
[maxtessfactor(64.0f)]
DS_INPUT main( 
	InputPatch<HS_INPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	DS_INPUT output;

	output.tangent = ip[i].tangent;
    output.normal = ip[i].normal;
    output.pos = ip[i].pos;
    output.pos.w = 1;
    output.tex_arr_idx = ip[i].tex_arr_idx;
    output.uv = ip[i].uv;
    output.world_pos = ip[i].world_pos;

	return output;
}
