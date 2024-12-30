Texture2DArray texture_arr_c : register(t0);
Texture2DArray texture_arr_n : register(t1);
Texture2DArray texture_arr_s : register(t2);
Texture2D texture_uvw : register(t3);
Texture2D texture_pos : register(t4);
Texture2D texture_tan : register(t5);
Texture2D texture_n : register(t6); // tbn 행렬을 위한 normal

SamplerState linear_sampler : register(s0);

cbuffer eye_pos : register(b0)
{
    float4 cam_pos;
}

struct PS_INPUT
{
    float4 pos : SV_Position;
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

static const float height_scale = 0.42;

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
        1.0 - texture_arr_n.Sample(linear_sampler, uvw).w;
    float current_layer_depth = 0.0f;
    
    [loop]
    while (current_layer_depth < current_depth_val)
    {
        current_uv -= delta_uv;
        current_layer_depth += layer_depth;
        current_depth_val =
            1.0 - texture_arr_n.Sample(linear_sampler, float3(current_uv, uvw.z)).w;
    }
    float2 prev_uv = current_uv + delta_uv;
    float after_depth = current_depth_val - current_layer_depth;
    float before_depth = 1.0 -
        texture_arr_n.Sample(linear_sampler, float3(prev_uv, uvw.z)).w;
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

float calcLOD(float3 pos, float3 t_normal)
{
    float distance = length(pos - cam_pos.xyz);
    float3 dir_view = normalize(cam_pos.xyz - pos);
    float dist_min = 5.0;
    float dist_max = 100.0 * abs(dot(t_normal, dir_view));
    float lod = (distance - dist_min) / (dist_max - dist_min);
    lod = saturate(lod) * 9.0;
    return lod;
}

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    float3 uvw = texture_uvw.Sample(linear_sampler, input.uv).xyz;
    if (uvw.x == 0 && uvw.y == 0 && uvw.z == 0)
        discard;
    float3 pos = texture_pos.Sample(linear_sampler, input.uv).xyz;
    float3 tangent = texture_tan.Sample(linear_sampler, input.uv).xyz;
    float3 t_normal = texture_n.Sample(linear_sampler, input.uv).xyz;
    float3 bitangent = cross(t_normal, tangent);
    bitangent = normalize(bitangent);
    float3x3 tbn = float3x3(tangent, bitangent, t_normal);

    float3 view_dir = calcViewDir(pos, tbn);
    float3 ori_uvw = uvw;
    uvw = parallaxOcclusionMapping(uvw, view_dir);
    float2 delta_uv = uvw.xy - ori_uvw.xy;
    float2 offset_xy = mul(delta_uv, float2x3(tangent, bitangent));
    pos += float3(offset_xy, 0);
    
    
    float lod = calcLOD(pos, t_normal);
    output.color = texture_arr_c.SampleLevel(linear_sampler, uvw, lod);
    if (uvw.z == 6 || uvw.z == 7 || uvw.z == 8)
        output.color = float4(float3(72.0, 181.0, 24.0) / 255.0, 1);
    
    output.w_pos = float4(pos, 1);
    float3 normal = texture_arr_n.SampleLevel(linear_sampler, uvw, lod).xyz;
    normal = 2 * normal - 1.0f;
    normal = normalize(mul(normal, tbn));
    output.w_normal = float4(normal, 1);
    float r = 1.0 - texture_arr_s.SampleLevel(linear_sampler, uvw, lod).r;
    float m = texture_arr_s.SampleLevel(linear_sampler, uvw, lod).g;
    float ao = texture_arr_s.SampleLevel(linear_sampler, uvw, lod).w;
    output.rma = float4(r, m, ao, 1);
    output.ssao_normal = float4(t_normal, 1);
	return output;
}