cbuffer CameraMat : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VertexInput
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    output.position = mul(input.position, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    
    output.color = input.color;
    return output;
}

float4 PS(VertexOutput input) : SV_Target
{
    return input.color;
}
