cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

cbuffer Bones : register(b1)
{
    matrix bone[32];
    matrix inv_bindpose[32];
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float4 weight : BLENDWEIGHT;
    int4 bone : BLENDINDICES;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
};


PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1);
    output.world_pos = float4(input.pos, 1.0);
    //output.normal = input.normal;
    output.normal = float3(0.0, 1.0, 0.0);
    output.uv = input.uv;

    //output.pos = mul(output.pos, bone[input.bone.x]);
    //output.pos = mul(output.pos, inv_bindpose[input.bone.x]);
    //output.pos = mul(inv_bindpose[input.bone.x], output.pos);
    //output.pos = mul(bone[input.bone.x], output.pos);
    //output.pos = mul(output.pos, transpose(bone[input.bone.x]));
    //output.pos = mul(output.pos, bone[input.bone.x]);

    float4 acc_pos = float4(0.0, 0.0, 0.0, 0.0);

    acc_pos += mul(mul(output.pos, inv_bindpose[input.bone.x]), bone[input.bone.x]) * input.weight.x;
    acc_pos += mul(mul(output.pos, inv_bindpose[input.bone.y]), bone[input.bone.y]) * input.weight.y;
    acc_pos += mul(mul(output.pos, inv_bindpose[input.bone.z]), bone[input.bone.z]) * input.weight.z;
    acc_pos += mul(mul(output.pos, inv_bindpose[input.bone.w]), bone[input.bone.w]) * input.weight.w;

    output.pos = acc_pos;

    output.pos = mul(output.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    return output;
}