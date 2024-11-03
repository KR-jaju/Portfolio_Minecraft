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
    if (color.r == 0 && color.g == 0 && color.b == 0)
        return float4(1, 0, 0, 1);
    return color;
}