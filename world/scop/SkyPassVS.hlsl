
cbuffer CameraMatrices : register(b0)
{
    matrix view;
    matrix projection;
    matrix view_projection;
    matrix view_inverse_transpose;
    matrix projection_inverse;
};

cbuffer WorldMatrix : register(b1)
{
    matrix world;
};

struct VS_INPUT
{
    float3 position : SV_Position;
};

struct PS_INPUT
{
    float4 position : SV_Position;
    float3 direction : POSITION;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    float4 os_position = float4(input.position, 0); // 화면에서 가장 멀리있는 점
    float4 ws_position = mul(os_position, world);
    float4 vs_position = mul(ws_position, view);
    
    vs_position = float4(vs_position.xyz * 200, 1.0);
    output.position = mul(vs_position, projection);
    output.direction = ws_position;
    return output;
}
