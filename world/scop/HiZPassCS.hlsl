Texture2D<float> input : register(t0);
RWTexture2D<float> output : register(u0);

cbuffer DepthBufferParams : register(b0)
{
    int4 dimension;
};

[numthreads(8, 8, 1)]
void main(uint3 dispatchID : SV_DispatchThreadID)
{
    int2 pixel = int2(dispatchID.xy);
    int2 input_dimension = dimension.xy;
    int2 output_dimension = dimension.zw;

    if (pixel.x >= output_dimension.x || pixel.y >= output_dimension.y)
        return;
    int2 base_pixel = pixel * 2; // 2x2 영역의 시작 좌표
    float max_depth = max(input.Load(int3(base_pixel, 0)),
        max(input.Load(int3(base_pixel + int2(1, 0), 0)),
            max(input.Load(int3(base_pixel + int2(0, 1), 0)), input.Load(int3(base_pixel + int2(1, 1), 0)))
        )
    );

    bool isEdgeX = (base_pixel.x + 3 == input_dimension.x);
    bool isEdgeY = (base_pixel.y + 3 == input_dimension.y);

    if (isEdgeX)
        max_depth = max(input.Load(int3(base_pixel + int2(2, 0), 0)), max(input.Load(int3(base_pixel + int2(2, 1), 0)), max_depth));
    if (isEdgeY)
        max_depth = max(input.Load(int3(base_pixel + int2(0, 2), 0)), max(input.Load(int3(base_pixel + int2(1, 2), 0)), max_depth));
    if (isEdgeX && isEdgeY)
        max_depth = max(input.Load(int3(base_pixel + int2(2, 2), 0)), max_depth);
    output[pixel] = max_depth;
}
