
cbuffer CameraMatrix : register(b0)
{
    matrix camera;
};


cbuffer WorldMatrix : register(b1)
{
    matrix world;
};


struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    uint texture_id : BLENDINDICES;
    //int type : TYPE;
    //float3 pos : POSITION;
    //float3 normal : NORMAL;
    //float2 uv : TEXCOORD;
    //int dir : DIRECTION;
};

struct PS_INPUT
{
    //int type : TYPE;
    float4 pos : SV_Position;
    //float3 normal : NORMAL;
    //float3 world_pos : POSITION;
    float3 uv : TEXCOORD;
    //int dir : DIRECTION;
};


PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    output.pos = float4(input.pos, 1);
    //output.world_pos = input.pos;
    //output.normal = float3(0, 0, 1);
        //input.normal;
    output.uv = float3(input.uv, input.texture_id);
    //output.dir = input.dir;
    //output.type = input.type;

    //output.pos = mul(output.pos, world);
    //output.pos = mul(output.pos, view);
    //output.pos = mul(output.pos, proj);
    output.pos = mul(output.pos, world);
    output.pos = mul(output.pos, camera);
    return output;
}
