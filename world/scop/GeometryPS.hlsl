SamplerState sampler0 : register(s0);
Texture2DArray texture_arr : register(t0);
Texture2D color_test : register(t1);

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 pbr_normal : PBR_NORMAL;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    int lod : LEVEL;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
    float4 position : SV_Target2;
    float4 pbr_normal : SV_Target3;
};

cbuffer eyePos : register(b0)
{
    float3 pos;
    float r;
    matrix view;
};

PS_OUTPUT main(PS_INPUT input)
{
    float4 color;
    float3 uvw;
    float offset = (input.type - 1) * 3;
    PS_OUTPUT output;
    
    output.normal = float4(mul(input.normal, (float3x3) view), 1);
    output.pbr_normal = 
        float4(mul(input.pbr_normal, (float3x3) view), 1);
    output.position = float4(input.world_pos, 1);
    output.position = mul(output.position, view);
    if (input.dir == 0 || input.dir == 1)
        uvw = float3(input.uv, input.dir + offset);
    else
        uvw = float3(input.uv, 2 + offset);
    
    float2 dvec = float2(input.world_pos.x - pos.x,
        input.world_pos.z - pos.z);
    float d = sqrt(pow(dvec.x, 2) + pow(dvec.y, 2));
    float dist = length(input.world_pos - pos);
    float distMin = 0.5;
    float distMax = 50.0;
    float lod = 5 * saturate((dist - distMin) / (distMax - distMin)); // 나중에
    //color = texture_arr.SampleLevel(sampler0, uvw, 0);
    if (input.dir)
        color = texture_arr.SampleLevel(sampler0, uvw, 0);
    else
        color = color_test.SampleLevel(sampler0, input.uv, input.lod);
    output.color = color;
    
    return output;
}