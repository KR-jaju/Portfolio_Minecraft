struct PS_INPUT
{
    float4 pos : SV_Position;
    int shadow_flag : SHADOW;
};

struct PS_OUT
{
    float4 shadow : SV_Target0;
};

PS_OUT main(PS_INPUT input)
{
    PS_OUT  output;
    float3 shadow = input.shadow_flag * float3(1, 1, 1);
	output.shadow  = float4(shadow, 1);
    return output;
}