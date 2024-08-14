Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

cbuffer PSContstant
{
    float3 eyePositionWorld;
    float dummy;
};

struct PSInput
{
    float4 positionScreen : SV_POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
    float4 lightDirection : POSITION;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 color = input.color;
    
    float3 eyePosNormal = normalize(eyePositionWorld);
    float3 vertexNormal = normalize(input.normal);
    
    float3 lightStrength = max(dot(eyePosNormal, vertexNormal), 0.0);
    color = color.xyz * lightStrength;
        
    return float4(color, 1.0);
}