struct PS_INPUT
{
    float4 pos : SV_POSITION;
    int shadow_flag : SHADOW;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    int r = input.shadow_flag;
    return float4(r, r, r, 1);
}