struct HS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
};

struct DS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
};

cbuffer ConstData : register(b0)
{
    float3 eye_pos;
}

struct PatchConstOutput
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

PatchConstOutput CalcHSPatchConstants(
	InputPatch<HS_INPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	
    PatchConstOutput output;
    float3 center = 0;
    center = 0.5 * (ip[1].pos.xyz + ip[2].pos.xyz);
    center = (1 / 3.0f) * ip[0].pos.xyz +
        (2 / 3.0f) * ip[0].pos.xyz;
    float dist = length(center - eye_pos);
    float dist_min = 0.5;
    float dist_max = 2;
    float tess = 32 * 
        saturate((dist_max - dist) / (dist_max - dist_min)) + 1;
    
    output.edges[0] = tess;
    output.edges[1] = tess;
    output.edges[2] = tess;
    output.inside = tess;
    return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
[maxtessfactor(64.0f)]
DS_INPUT main( 
	InputPatch<HS_INPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	DS_INPUT output;

	output.dir = ip[i].dir;
    output.normal = ip[i].normal;
    output.pos = ip[i].pos;
    output.type = ip[i].type;
    output.uv = ip[i].uv;
    output.world_pos = ip[i].world_pos;

	return output;
}
