SamplerState sampler0 : register(s0);
Texture2D main_texture : register(t0);

struct PS_INPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 albedo_metallic : SV_Target0;
    float2 normal : SV_Target1;
};

cbuffer eyePos : register(b0)
{
    float3 pos;
    float r;
    matrix view;
};

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    //output.normal = float4(mul(input.normal, (float3x3) view), 1);
    //output.position = float4(input.world_pos, 1);
    //output.position = mul(output.position, view);

    //float2 dvec = float2(input.world_pos.x - pos.x,
    //    input.world_pos.z - pos.z);
    //float d = sqrt(pow(dvec.x, 2) + pow(dvec.y, 2));
    //float dist = pos.y - input.world_pos.y;
    //float distMin = 10.0;
    //float distMax = 50.0;
    //float lod = 5 * saturate((dist - distMin) / (distMax - distMin)); // ³ªÁß¿¡
    //input.uv.y = 1.0 - input.uv.y;
    //color = main_texture.SampleLevel(sampler0, input.uv, lod);
    ////color = main_texture.Sample(sampler0, input.uv);
    //output.color = color;
    //output.color = main_texture.Sample(sampler0, input.uv);
    //int2 uv = int2(frac(input.uv) * 16);
    int2 uv = int2(saturate(float2(0.0, 1.0) + float2(1.0, -1.0) * input.uv) * 64);
    float3 vs_normal = normalize(input.normal);

    output.albedo_metallic = main_texture.Load(int3(uv, 0));
    output.normal = vs_normal.xy;

    //output.color = float4(1.0, 1.0, 1.0, 1.0);
    //output.color = texture_array.Sample(sampler0, input.uvw);
    //output.color = float4(input.uv, 1.0, 1.0);

    return output;
}