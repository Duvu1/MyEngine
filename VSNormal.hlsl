cbuffer MVP : register(b0)
{
    matrix model;
    matrix inverseTranspose;
    matrix view;
    matrix projection;
};

cbuffer NormalConstant : register(b1)
{
    float scale;
    float3 dummy;
};

struct VSInput
{
    float3 positionWorld : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VSOutput
{
    float4 positionScreen : SV_POSITION;
    float3 color : COLOR;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    float4 pos = float4(input.positionWorld, 1.0);
    
    float4 normal = float4(input.normal, 0.0);
    normal = mul(normal, inverseTranspose);
    
    float3 normalWorld = normalize(normal.xyz);
    
    float t = input.texcoord.x;
    
    pos = mul(pos, model);
    pos.xyz += normalWorld * t * scale;
    
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.positionScreen = pos;
    output.color = float3(0.0, 1.0, 0.0) * (1.0 - t) + float3(0.0, 0.5, 1.0) * t;
    
    return output;
}