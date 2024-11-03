struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D sun_moon : register(t0);
Texture2D skybox : register(t1);
SamplerState sampler0 : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 color;
    color = sun_moon.Sample(sampler0, input.uv).rgb;
    color += skybox.Sample(sampler0, input.uv).rgb;
    return float4(color, 1);
}