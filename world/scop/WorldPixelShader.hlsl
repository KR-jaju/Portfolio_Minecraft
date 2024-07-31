SamplerState sampler0 : register(s0);
Texture2D g_texture[6] : register(t0);


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
    for (int i = 0; i < 6; i++)
    {
        if (input.dir == i)
        {
            color = g_texture[i].Sample(sampler0, input.uv);
            break;
        }
    }
    return color;
}