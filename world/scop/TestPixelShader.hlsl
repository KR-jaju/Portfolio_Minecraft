SamplerState sampler0 : register(s0);
Texture2DArray texture_arr : register(t0);

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    //int x_pos : XPOS;
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
    
    if (input.type == 2)
        color = float4(1, 0, 0, 1);
    else
    {
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
    
    
        if (d <= r)
            color = texture_arr.SampleLevel(sampler0, uvw, lod);
        else
            color = texture_arr.SampleLevel(sampler0, uvw, lod) * float4(0.6, 0.6, 0.6, 1.0);
    }
    return color;
}