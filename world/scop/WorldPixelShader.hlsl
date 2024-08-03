SamplerState sampler0 : register(s0);
Texture2DArray texture_arr : register(t0);

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
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

    color = texture_arr.Sample(sampler0, uvw);
    
    return color;
}