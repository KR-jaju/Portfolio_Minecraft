cbuffer CameraData : register(b0)
{
    matrix view;
    matrix proj;
}

cbuffer TransformData : register(b1)
{
    row_major matrix matWorld;
}
struct VS_INPUT
{
    float3 pos : POSITION;
    float4 col : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float4 col : COLOR;
};


VS_OUTPUT main( VS_INPUT input )
{
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1.f);
    output.col = input.col;
    
    output.pos = mul(output.pos, matWorld);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);

    //output.pos = mul(output.pos, mvp);
    //matrix mvp = mul(proj, view);
    //mvp = mul(mvp, model);
    //output.pos = mul(mvp, output.pos);
    //output.pos = mul(model, output.pos);
    //output.pos = mul(view, output.pos);
    //output.pos = mul(proj, output.pos);
    return output;
}