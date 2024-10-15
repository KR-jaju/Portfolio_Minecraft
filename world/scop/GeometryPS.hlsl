SamplerState sampler0 : register(s0);
Texture2DArray texture_arr : register(t0);

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
    float4 position : SV_Target2;
};

cbuffer eyePos : register(b0)
{
    float3 pos;
    float r;
};

PS_OUTPUT main(PS_INPUT input)
{
    float4 color;
    float3 uvw;
    float offset = (input.type - 1) * 3;
    PS_OUTPUT output;
    
    output.normal = float4(input.normal, 1);
    output.position = float4(input.world_pos, 1);
    if (input.dir == 0 || input.dir == 1)
        uvw = float3(input.uv, input.dir + offset);
    else
        uvw = float3(input.uv, 2 + offset);
    
    float2 dvec = float2(input.world_pos.x - pos.x,
        input.world_pos.z - pos.z);
    float d = sqrt(pow(dvec.x, 2) + pow(dvec.y, 2));
    float dist = pos.y - input.world_pos.y;
    float distMin = 10.0;
    float distMax = 50.0;
    float lod = 5 * saturate((dist - distMin) / (distMax - distMin)); // 나중에
    color = texture_arr.SampleLevel(sampler0, uvw, 0);
    output.color = color;
    
    return output;
}