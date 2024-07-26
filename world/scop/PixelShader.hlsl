struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float4 col : COLOR;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    return float4(input.col.xyz, 1);
}