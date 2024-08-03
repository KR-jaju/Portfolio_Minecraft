SamplerState sampler0 : register(s0);
Texture2D g_texture[3] : register(t0);

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
    
    /*
    if (input.dir == 0 || input.dir == 1)
        uvw = float3(input.uv, input.dir);
    else
        uvw = float3(input.uv, 2);
    uvw = float3(input.uv, 0);
    */
    if (input.dir == 0)
        color = g_texture[0].Sample(sampler0, input.uv);
    else if (input.dir == 1)
        color = g_texture[1].Sample(sampler0, input.uv);
    else
        color = g_texture[2].Sample(sampler0, input.uv);
    return color;
}