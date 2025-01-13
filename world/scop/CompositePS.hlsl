struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

Texture2D accum : register(t0);
Texture2D reveal : register(t1);

SamplerState sampler0 : register(s0);

cbuffer constant_data : register(b1)
{
    static const float epsilon = 0.00001f;
};

// calculate floating point numbers equality accurately
bool isApproximatelyEqual(float a, float b)
{
    return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * epsilon;
}

// get the max value between three values
float max3(float3 v)
{
    return max(max(v.x, v.y), v.z);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float revealage = reveal.Sample(sampler0, input.uv).r;
	
    if (isApproximatelyEqual(revealage, 1.0f)) 
        discard;
 
    float4 accumulation = accum.Sample(sampler0, input.uv);
	
    float3 abs_accum = float3(abs(accumulation.r),
        abs(accumulation.g), abs(accumulation.b));
    if (isinf(max3(abs_accum)))
        accumulation.rgb = float3(accumulation.a, accumulation.a, accumulation.a);

    float3 average_color = accumulation.rgb / 
        max(accumulation.a, epsilon);

    return float4(average_color, 1.0f - revealage);
}