struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 view_pos : VIEW_POSITION;
    float4 clip_pos : CLIP_POSITION;
    float4 color : COLOR;
};


struct PS_OUTPUT
{
    float4 accum : SV_Target0;
    float reveal : SV_Target1;
};

Texture2D tex_depth : register(t0);
SamplerState sampler0 : register(s0);


float4 accum_0(PS_INPUT input)
{
    // weight function
    float z_near = 0.01;
    float z_far = 1000;
    float z = z_near * z_far * (input.view_pos.z - z_far) / (z_near - z_far);
    float weight = 
        clamp(pow(min(1.0, input.color.a * 10.0) + 0.01, 3.0) * 1e8 * 
            pow(1.0 - z * 0.9, 3.0), 0.01, 3000);
    
    float4 res = 
        float4(input.color.rgb * input.color.a, input.color.a) * weight;
    return res;
}

float4 accum_1(PS_INPUT input)
{
    // weight function
    float z_near = 0.01;
    float z_far = 1000;
    float z = z_near * z_far * (input.view_pos.z - z_far) / (z_near - z_far);
    float weight = input.color.a * max(0.01, 3000 * pow(1 - z, 3));
    
    float4 res = input.color * weight;
    return res;
}

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    float2 texcoord;
    float3 ndc_p = input.clip_pos.xyz / input.clip_pos.w;
    texcoord.x = (ndc_p.x + 1.0f) * 0.5f;
    texcoord.y = -1.0f * (ndc_p.y + 1.0f) * 0.5f;
    float std_z = tex_depth.Sample(sampler0, texcoord).r;
    if (ndc_p.z > std_z)
        discard;
    
	// store pixel color accumulation
    output.accum = accum_0(input);
    //output.accum = accum_1(input);
    
	// store pixel revealage threshold
    output.reveal = input.color.a;
    
	return output;
}