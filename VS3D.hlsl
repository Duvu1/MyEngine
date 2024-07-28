cbuffer MVP : register(b0)
{
    matrix view;
};

struct VSInput
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    
    output.position = mul(input.position, view);
    output.color = input.color;
    output.uv = input.uv;
    
    return output;
}