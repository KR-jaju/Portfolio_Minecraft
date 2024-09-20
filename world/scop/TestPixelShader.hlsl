SamplerState sampler0 : register(s0);
SamplerState sampler1 : register(s1);
Texture2DArray texture_arr : register(t0);
Texture2D depth_map : register(t1);

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    float3 l_pos : L_POSITION;
};

cbuffer eyePos : register(b0)
{
    float3 pos;
    float r;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color;
    float3 uvw;
    float offset = (input.type - 1) * 3;
    
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
    float lod = 5 * saturate((dist - distMin) / (distMax - distMin));
    
    
    input.l_pos.y *= -1;
    input.l_pos.xy = (input.l_pos.xy + 1) * 0.5;
    float depth = depth_map.Sample(sampler1, input.l_pos.xy).r;
    color = texture_arr.SampleLevel(sampler0, uvw, lod);
    if (depth + 0.001 < input.l_pos.z)
        return float4(0, 0, 0, 1);
    return color;
}