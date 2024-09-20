struct PS_INPUT
{
    float4 ndc_pos : SV_Position;
    float4 clip_pos : POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(1, 1, 1, 1);
}