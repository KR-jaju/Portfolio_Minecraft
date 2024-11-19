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


float4 main(PS_INPUT input) : SV_TARGET
{
    float3 a_color = 
        ambient_color.Sample(sampler0, input.uv).rgb;
    //return float4(a_color, 1);
    float3 d_color = 
        directional_color.Sample(sampler0, input.uv).rgb;
    float4 color = float4(a_color + d_color, 1);
    //color = float4(a_color, 1);
    if (color.r == 0 && color.g == 0 && color.b == 0)
        return cube_map.Sample(sampler0, input.uv);
    
    float sp = shadow_map.Sample(sampler0, input.uv).r;
    sp /= 15.f;
    sp = max(sp, 0.1);
    float4 ssao = ssao_map.Sample(sampler0, input.uv);
    float4 res = float4(sp, sp, sp, 1) * ssao;
    color = float4(a_color.xyz + d_color.xyz, 1);
    color = clamp(color, 0.0, 1000.0);
    //return color;
    return color * res;
}