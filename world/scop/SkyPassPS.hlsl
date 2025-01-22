
SamplerState sampler0 : register(s0);
TextureCube cube : register(t0);

struct PS_INPUT
{
    float4 position : SV_Position;
    float3 direction : POSITION;
};

float4 main(PS_INPUT input) : SV_Target
{
    float3 direction = normalize(input.direction);
    float4 sky_color = cube.Sample(sampler0, direction);

    return float4(sky_color.xyz, 1.0);
}