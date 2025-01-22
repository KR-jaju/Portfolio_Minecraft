cbuffer CameraMatrices : register(b0)
{
    matrix view;
    matrix projection;
    matrix view_projection;
    matrix view_inverse_transpose;
};

cbuffer WorldMatrix : register(b1)
{
    matrix world;
};

cbuffer Bindposes : register(b2)
{
    matrix bindposes[32];
};

cbuffer Bones : register(b3)
{
    matrix bone[32];
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
    float4 weight : BLENDWEIGHT;
    int4 bone : BLENDINDICES;
};

//struct PS_INPUT
//{
//    float4 pos : SV_Position;
//    float3 normal : NORMAL;
//    float3 world_pos : POSITION;
//    float2 uv : TEXCOORD;
//};

struct PS_INPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    float4  ls_position = float4(input.pos, 1.0);
    float3  bs_position = float4(0.0, 0.0, 0.0, 0.0);
    bs_position += mul(mul(ls_position, bindposes[input.bone.x]), bone[input.bone.x]) * input.weight.x;
    bs_position += mul(mul(ls_position, bindposes[input.bone.y]), bone[input.bone.y]) * input.weight.y;
    bs_position += mul(mul(ls_position, bindposes[input.bone.z]), bone[input.bone.z]) * input.weight.z;
    bs_position += mul(mul(ls_position, bindposes[input.bone.w]), bone[input.bone.w]) * input.weight.w;
    //position = object_position;
    bs_position = bs_position.xzy * float3(1.0, -1.0, 1.0); // 블렌더 루트 본 그냥 하드코딩
    float4 ws_position = mul(float4(bs_position, 1.0), world);
    float4 vs_position = mul(ws_position, view);

    output.position = mul(vs_position, projection);
    output.uv = input.uv;
    output.normal = mul(input.normal, view_inverse_transpose);
    return output;
}
