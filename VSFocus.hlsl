cbuffer MVP : register(b0)
{
    matrix model;
    matrix inverseTranspose;
    matrix view;
    matrix projection;
}

cbuffer Focus : register(b1)
{
    matrix modelFocus;
}

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VSOutput
{
    float4 positionScreen : SV_POSITION;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    
    float4 pos = float4(input.position, 1.0);
    
    pos = mul(pos, modelFocus);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.positionScreen = pos;

    return output;
}