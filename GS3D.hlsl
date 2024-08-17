cbuffer NormalConstant : register(b0)
{
    float scale;
};

struct GSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
};

struct GSOutput
{
    
};