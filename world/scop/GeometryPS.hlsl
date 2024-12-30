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
    float4 w_pos : SV_Target1;
    float4 w_normal : SV_Target2;
    float4 rma : SV_Target3;
    float4 ssao_normal : SV_Target4;
    float4 uvw : SV_Target5;
    float4 tan : SV_Target6;
    float4 tan_n : SV_Target7; // bitangent 계산을 위한 normal
};

PS_OUTPUT main(PS_INPUT input)
{
    float3 uvw;
    PS_OUTPUT output;
    
    output.w_pos = float4(input.w_pos, 1);
    uvw = float3(input.uv, input.tex_arr_idx);
    output.color = texture_arr.Sample(sampler_linear, uvw);
    if (output.color.w < 0.8)
        discard;
    output.color.w = 1;
    if (uvw.z == 6 || uvw.z == 7 || uvw.z == 8)
        output.color = float4(float3(72.0, 181.0, 24.0) / 255.0, 1);
    
    output.uvw = float4(uvw, 1);
    float3 tangent = normalize(input.tangent -
        dot(input.tangent, input.normal) * input.normal);
    output.tan = float4(tangent, 1);
    output.tan_n = float4(input.normal, 1);
    float3 bitangent = cross(input.normal, tangent);
    bitangent = normalize(bitangent);
    float3x3 tbn = float3x3(tangent, bitangent, input.normal);
    float3 normal = texture_arr_n.Sample(sampler_linear, uvw).xyz;
    
    normal = 2 * normal - 1.0;
    normal = normalize(mul(normal, tbn));
    output.w_normal = float4(normal, 1);
    float r = 1.0 - texture_arr_s.Sample(sampler_linear, uvw).r;
    float m = texture_arr_s.Sample(sampler_linear, uvw).g;
    float ao = texture_arr_s.Sample(sampler_linear, uvw).w;
    output.rma = float4(r, m, ao, 1);
    float h = texture_arr_n.Sample(sampler_linear, uvw).w;
    output.ssao_normal = float4(input.normal, 1);
    return output;
}