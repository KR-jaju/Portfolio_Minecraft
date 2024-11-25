struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D ambient_color : register(t0);
Texture2D directional_color : register(t1);
Texture2D shadow_map : register(t2);
Texture2D ssao_map : register(t3);
Texture2D cube_map : register(t4);

SamplerState sampler0 : register(s0);

float3 LinearToneMapping(float3 color)
{
    float3 invGamma = float3(1, 1, 1) / 2.2f;

    color = clamp(color, 0., 1.);
    color = pow(color, invGamma);
    return color;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 a_color = 
        ambient_color.Sample(sampler0, input.uv).rgb;
    a_color = LinearToneMapping(a_color);
    float3 d_color = 
        directional_color.Sample(sampler0, input.uv).rgb;
    d_color = LinearToneMapping(d_color);
    float4 color = float4(a_color + d_color, 1);
    if (color.r == 0 && color.g == 0 && color.b == 0)
        return float4(LinearToneMapping(
            cube_map.Sample(sampler0, input.uv).rgb), 1);
    
    float sp = shadow_map.Sample(sampler0, input.uv).r;
    sp /= 15.f;
    sp = max(sp, 0.1);
    float4 ssao = ssao_map.Sample(sampler0, input.uv);
    ssao = 1; // test
    float4 res = float4(sp, sp, sp, 1) * ssao;
    if (sp >= 0.9f)
        color = float4(a_color.xyz * ssao.xyz + d_color.xyz, 1);
    else
        color = float4(a_color.xyz, 1) * ssao;
    color = clamp(color, 0.0, 1000.0);
    //color = float4(LinearToneMapping(color.rgb), 1);
    return color * sp;
}