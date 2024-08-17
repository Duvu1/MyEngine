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
    //float4 focusPositionScreen : SV_Position;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 lightDirection : POSITION;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 color = float3(0.5, 0.5, 0.5);
    
    float3 eyePosNormal = normalize(eyePositionWorld);
    float3 vertexNormal = normalize(input.normal);
    
    float lightStrength = max(dot(eyePosNormal, vertexNormal), 0.0);
    color = color * lightStrength;
        
    return float4(color, 1.0);
}