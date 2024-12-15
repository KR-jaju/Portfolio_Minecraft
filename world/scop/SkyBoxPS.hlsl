struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 w_pos : POSITION;
};

TextureCube cube : register(t0);
SamplerState sampler0 : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = cube.Sample(sampler0, input.w_pos);
    return color;
}