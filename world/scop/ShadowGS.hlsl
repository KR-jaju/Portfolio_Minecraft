struct MVP
{
    matrix world;
    matrix view;
    matrix proj;
};
StructuredBuffer<MVP> mvp_arr : register(t0);

struct PS_INPUT
{
	float4 pos : SV_POSITION;
    uint idx : SV_RenderTargetArrayIndex;
};

[maxvertexcount(9)]
void main(
	triangle float4 input[3] : SV_POSITION, 
	inout TriangleStream< PS_INPUT > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		PS_INPUT element;
        element.idx = i;
        for (int j = 0; j < 3; j++)
        {
            element.pos = input[j];
            element.pos = mul(element.pos, mvp_arr[i].view);
            element.pos = mul(element.pos, mvp_arr[i].proj);
		    output.Append(element);
        }
        output.RestartStrip();
    }
}