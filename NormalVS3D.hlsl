cbuffer MVP : register(b0)
{
    matrix model;
    matrix inverseTranspose;
    matrix view;
    matrix projection;
    float scale;
};

struct VSInput
{
    float3 position : POSITION;
    float3 color : COLOR;
    float3 normal : NORMAL;
};

float3 main(VSInput input)
{
    float4 pos = float4(input.position, 1.0);
    pos = pos + lerp(0.0, 1.0, input.normal);
    
    pos = mul(pos, model);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.position = pos;
    
    return output;
}