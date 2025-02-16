struct PSInput
{
    float4 pos : SV_Position; // pos.z°¡ ÇöÀç ÇÈ¼¿ÀÇ ±íÀÌ
};

float main(PSInput input) : SV_Target0
{
    float depth = input.pos.z;

    return depth;
}
