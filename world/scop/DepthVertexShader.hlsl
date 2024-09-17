cbuffer ConstantBuffer : register(b0)
{
    matrix mat1;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}