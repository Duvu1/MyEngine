cbuffer MVP : register(b0)
{
    matrix model;
    matrix inverseTranspose;
    matrix view;
    matrix projection;
};

struct VSInput
{
    float3 positionWorld : POSITION;
    float3 color : COLOR;
    float3 normal : NORMAL;
};

struct VSOutput
{
    float4 positionScreen : SV_POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
    float4 lightDirection : POSITION;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    float4 pos = float4(input.positionWorld, 1.0);
    
    pos = mul(pos, model);    
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.positionScreen = pos;
    
    float4 normal = float4(input.normal, 0.0);
    output.normal = mul(normal, inverseTranspose).xyz;
    output.normal = normalize(output.normal);
    
    output.color = input.color;
    
    float4 lightPos = float4(0.0, 0.0, -5.0, 1.0);
    lightPos = mul(lightPos, view);
    lightPos = mul(lightPos, projection);
    output.lightDirection = float4(lightPos.xyz, 0.0);
    
    return output;
}