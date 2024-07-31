struct PS_INPUT
{
    int type : TYPE;
    float4 pos : SV_Position;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return input.color;
}