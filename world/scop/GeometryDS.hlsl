cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
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

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
};

struct PatchConstOutput
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
PS_INPUT main(
	PatchConstOutput input,
	float3 uvw : SV_DomainLocation,
	const OutputPatch<DS_INPUT, NUM_CONTROL_POINTS> patch)
{
	PS_INPUT output;

    float3 position;
    position = uvw.x * patch[0].pos.xyz + uvw.y * patch[1].pos.xyz 
        + patch[2].pos.xyz * uvw.z;
    output.pos = float4(position, 1);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    output.dir = patch[0].dir;
    output.normal = patch[0].normal;
    output.world_pos = position;
    output.type = patch[0].type;
    output.uv = uvw.x * patch[0].uv + uvw.y * patch[1].uv +
        uvw.z * patch[2].uv;
    return output;
}
