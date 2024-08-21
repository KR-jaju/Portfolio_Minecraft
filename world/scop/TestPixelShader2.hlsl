SamplerState sampler0 : register(s0);
Texture2DArray texture_arr : register(t0);

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
};

cbuffer eyePos : register(b0)
{
    float3 pos;
    float dummy;
    float r;
    //float3 dummys;
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
    
    if (d <= r)
        color = texture_arr.Sample(sampler0, uvw);
    else
        color = texture_arr.Sample(sampler0, uvw) * 0.6;
    return color;
}