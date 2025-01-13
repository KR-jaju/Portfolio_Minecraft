struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D terrain_tex : register(t0);
Texture2D sky_tex : register(t1);

SamplerState sampler0 : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = terrain_tex.Sample(sampler0, input.uv);
    if (color.r == 0 && color.b == 0 && color.g == 0)
        color = sky_tex.Sample(sampler0, input.uv);
	return color;
}