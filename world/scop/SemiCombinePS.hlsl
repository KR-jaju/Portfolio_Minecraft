struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D sun_moon : register(t0);
Texture2D blur : register(t1);
SamplerState sampler0 : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 color_sm = sun_moon.Sample(sampler0, input.uv).xyz;
    float3 color_blur = blur.Sample(sampler0, input.uv).xyz;
    float3 color = color_sm + color_blur * 0.62;
    return float4(color, 1);
}