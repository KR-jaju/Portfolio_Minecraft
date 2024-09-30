SamplerState sampler0 : register(s0);
SamplerState sampler1 : register(s1);
Texture2DArray texture_arr : register(t0);
Texture2D depth_map : register(t1);

struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    int dir : DIRECTION;
    int shadow_flag : SHADOW;
};

cbuffer eyePos : register(b0)
{
    float3 pos;
    float r;
};

cbuffer lightMat : register(b1)
{
    matrix l_world;
    matrix l_view;
    matrix l_proj;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color;
    float3 uvw;
    float offset = (input.type - 1) * 3;
    
    if (input.dir == 0 || input.dir == 1)
        uvw = float3(input.uv, input.dir + offset);
    else
        uvw = float3(input.uv, 2 + offset);

    float2 dvec = float2(input.world_pos.x - pos.x, 
        input.world_pos.z - pos.z);
    float d = sqrt(pow(dvec.x, 2) + pow(dvec.y, 2));
    float dist = pos.y - input.world_pos.y;
    float distMin = 10.0;
    float distMax = 50.0;
    float lod = 5 * saturate((dist - distMin) / (distMax - distMin));
    color = texture_arr.SampleLevel(sampler0, uvw, lod);
    
    
    //shadow map
    /*
    float move = 0.1;
    input.world_pos += move * input.normal;
    float4 lpos = float4(input.world_pos, 1);
    lpos = mul(lpos, l_view);
    lpos = mul(lpos, l_proj);
    lpos.y *= -1;
    lpos.xy = (lpos.xy + 1) * 0.5;
    float depth = depth_map.Sample(sampler1, lpos.xy).r;
    float3 light_dir = float3(0, -1, 0);
    float bias = 0.005 * tan(acos(dot(input.normal, light_dir)));
    bias = clamp(bias, 0, 0.0001);
    if (depth + bias < lpos.z)
        return color * float4(0.3, 0.3, 0.3, 1);
    return color;
    */
    
    float sp = input.shadow_flag;
    sp /= 15.f;
    sp = max(sp, 0.3);
    return color * float4(sp, sp, sp, 1);
}