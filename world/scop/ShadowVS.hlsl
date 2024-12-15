struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    int shadow_flag : SHADOW;
};


float4 main( VS_INPUT input ) : SV_Position
{
    return float4(input.pos, 1);
}