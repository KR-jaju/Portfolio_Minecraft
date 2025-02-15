SamplerState sampler0 : register(s0);
Texture2D hdr_texture : register(t0);

struct PSInput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float3 ReinhardToneMapping(float3 color) {
    return color / (color + 0.18); // 간단한 Reinhard 톤 매핑
}

float4 main(PSInput input) : SV_TARGET
{
    float3  hdr_color = hdr_texture.Sample(sampler0, input.uv).rgb;
    float3  tone_mapped = ReinhardToneMapping(hdr_color);
    float3  gamma_corrected = pow(tone_mapped, 1 / 2.2);

    return float4(gamma_corrected, 1.0);
}