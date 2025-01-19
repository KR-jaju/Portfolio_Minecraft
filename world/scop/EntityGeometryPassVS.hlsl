cbuffer CameraMatrix : register(b0)
{
    matrix camera;
};


cbuffer WorldMatrix : register(b1)
{
    matrix world;
};

cbuffer Bones : register(b2)
{
    matrix bone[32];
};

cbuffer InverseBindpose : register(b3)
{
    matrix bindposes[32];
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
    //int type : TYPE;
    float4 pos : SV_Position;
    //float3 normal : NORMAL;
    //float3 world_pos : POSITION;
    float2 uv : TEXCOORD;
    //int dir : DIRECTION;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    float4  object_position = float4(input.pos, 1.0);
    float3  position = float4(0.0, 0.0, 0.0, 0.0);

    //position += mul(mul(object_position, bindposes[input.bone.x]), bone[input.bone.x]) * input.weight.x;
    //position += mul(mul(object_position, bindposes[input.bone.y]), bone[input.bone.y]) * input.weight.y;
    //position += mul(mul(object_position, bindposes[input.bone.z]), bone[input.bone.z]) * input.weight.z;
    //position += mul(mul(object_position, bindposes[input.bone.w]), bone[input.bone.w]) * input.weight.w;

    position = object_position;
    
    output.uv = input.uv;
    output.pos = mul(float4(position, 1.0), world);
    output.pos = mul(output.pos, camera);
    return output;
}
