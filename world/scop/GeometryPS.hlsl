Texture2DArray texture_arr : register(t0);
Texture2DArray texture_arr_s : register(t1);
Texture2DArray texture_arr_n : register(t2);

SamplerState sampler_linear : register(s0);

struct PS_INPUT
{
    int tex_arr_idx : INDEX;
    float4 pos : SV_Position;
    float3 w_pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
    float4 position : SV_Target2;
    float4 w_pos : SV_Target3;
    float4 w_normal : SV_Target4;
    float4 metallic : SV_Target5;
    float4 roughness : SV_Target6;
};

cbuffer eyePos : register(b0)
{
    matrix view;
};

PS_OUTPUT main(PS_INPUT input)
{
    float3 uvw;
    PS_OUTPUT output;
    
    output.position = float4(input.w_pos, 1);
    output.w_pos = output.position;
    output.position = mul(output.position, view);
    uvw = float3(input.uv, input.tex_arr_idx);
    float3 normal = texture_arr_n.Sample(sampler_linear, uvw).xyz;
    normal = 2 * normal - 1.0;
    float3 tangent = normalize(input.tangent -
        dot(input.tangent, input.normal) * input.normal);
    float3 bitangent = cross(input.normal, tangent);
    bitangent = normalize(bitangent);
    float3x3 tbn = float3x3(tangent, bitangent, input.normal);
    normal = normalize(mul(normal, tbn));
    output.w_normal = float4(normal, 1);
    output.normal = float4(mul(input.normal, (float3x3) view), 1);
    float r = texture_arr_s.Sample(sampler_linear, uvw).r;
    float m = texture_arr_s.Sample(sampler_linear, uvw).g;
    output.roughness = float4(r, r, r, 1);
    output.metallic = float4(m, m, m, 1);
    output.color = 
        float4(texture_arr.Sample(sampler_linear, uvw).rgb, 1);
    return output;
}