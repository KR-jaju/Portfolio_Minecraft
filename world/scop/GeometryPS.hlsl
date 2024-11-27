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
};

static float height_scale = 0.42;

cbuffer c : register(b0)
{
    float4 cam_pos;
}

float3 parallaxOcclusionMapping(float3 uvw, float3 view_dir)
{
    float2 uv = uvw.xy;
    const float min_layer = 8;
    const float max_layer = 32;
    float num_layer = lerp(max_layer, min_layer,
        dot(float3(0, 0, 1), view_dir));
    float layer_depth = 1.0 / num_layer;
    float2 delta_uv = view_dir.xy / view_dir.z * height_scale;
    delta_uv /= num_layer;
    
    float2 current_uv = uv;
    float current_depth_val =
        1.0 - texture_arr_n.Sample(sampler_linear, uvw).w;
    float current_layer_depth = 0.0f;
    
    [loop]
    while (current_layer_depth < current_depth_val)
    {
        current_uv -= delta_uv;
        current_layer_depth += layer_depth;
        current_depth_val =
            1.0 - texture_arr_n.Sample(sampler_linear, 
            float3(current_uv, uvw.z)).w;
    }
    float2 prev_uv = current_uv + delta_uv;
    float after_depth = current_depth_val - current_layer_depth;
    float before_depth = 1.0 -
        texture_arr_n.Sample(sampler_linear, float3(prev_uv, uvw.z)).w;
    before_depth -= (current_layer_depth - layer_depth);
    float weight = after_depth / (after_depth - before_depth);
    float2 adj_uv = prev_uv * weight + current_uv * (1.0 - weight);
    return float3(adj_uv, uvw.z);
}

float3 calcViewDir(float3 pos, float3x3 tbn)
{
    float3x3 inv_tbn = transpose(tbn);
    float3 tan_pos = mul(pos, inv_tbn);
    float3 tan_eye = mul(cam_pos.xyz, inv_tbn);
    float3 view_dir = normalize(tan_eye - tan_pos);
    return view_dir;
}

PS_OUTPUT main(PS_INPUT input)
{
    float3 uvw;
    PS_OUTPUT output;
    
    output.w_pos = float4(input.w_pos, 1);
    uvw = float3(input.uv, input.tex_arr_idx);
    float3 tangent = normalize(input.tangent -
        dot(input.tangent, input.normal) * input.normal);
    float3 bitangent = cross(input.normal, tangent);
    bitangent = normalize(bitangent);
    float3x3 tbn = float3x3(tangent, bitangent, input.normal);
    
    // parallax occlusion mapping 안할거면 끄기
    bool parallax_flag = true;
    if (parallax_flag)
    {
        float2 origin_uv = uvw.xy;
        uvw = parallaxOcclusionMapping(uvw, 
            calcViewDir(input.w_pos, tbn));
        float2 delta_uv = uvw.xy - origin_uv;
        float2 offset_xy = mul(delta_uv, float2x3(tangent, bitangent));
        output.w_pos += float4(offset_xy, 0, 0);
    }
    
    float3 normal = texture_arr_n.Sample(sampler_linear, uvw).xyz;
    normal = 2 * normal - 1.0;
    normal = normalize(mul(normal, tbn));
    output.w_normal = float4(normal, 1);
    float r = 1.0 - texture_arr_s.Sample(sampler_linear, uvw).r;
    float m = texture_arr_s.Sample(sampler_linear, uvw).g;
    float ao = texture_arr_s.Sample(sampler_linear, uvw).w;
    output.rma = float4(r, m, ao, 1);
    output.color = 
        float4(texture_arr.Sample(sampler_linear, uvw).rgb, 1);
    float h = texture_arr_n.Sample(sampler_linear, uvw).w;
    output.ssao_normal = float4(input.normal, 1);
    return output;
}