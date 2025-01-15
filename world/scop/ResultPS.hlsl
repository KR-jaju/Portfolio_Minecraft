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
Texture2D c_shadow_map : register(t5);

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
    float4 d_c = 
        directional_color.Sample(sampler0, input.uv);
    float3 d_color = d_c.rgb;
    float cave_shadow = c_shadow_map.Sample(sampler0, input.uv).r;
    cave_shadow /= 15.0f;
    
    d_color = LinearToneMapping(d_color);
    float4 color = float4(a_color + d_color, 1);
    if (color.r == 0 && color.g == 0 && color.b == 0)
        return float4(LinearToneMapping(
            cube_map.Sample(sampler0, input.uv).rgb), 1);
    
    float sp = shadow_map.Sample(sampler0, input.uv).r;
    
    float4 ssao = ssao_map.Sample(sampler0, input.uv);
    color = clamp(color, 0.0, 1000.0);
    color = float4(a_color.xyz * ssao.r * cave_shadow + d_color.xyz * sp, 1);
    return color;
}