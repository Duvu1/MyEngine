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
    
    float3 lightDirection = float3(0.0, 0.0, 1.0);
    //float3 lightDirection1 = float3(-1.0, 0.0, 0.0);
    //float3 lightDirection2 = float3(1.0, 0.0, 0.0);
    
    //float3 lightStrength = clamp(dot(-lightDirection, input.normal), 0.0, 1.0);
    //float3 lightStrength1 = clamp(dot(-lightDirection1, input.normal), 0.0, 1.0);
    //float3 lightStrength2 = clamp(dot(-lightDirection2, input.normal), 0.0, 1.0);
    
    float3 lightStrength = max(dot(eyePositionWorld.xyz, input.normal), 0.0);
    
    color = clamp(color.xyz * (lightStrength / 3.0), 0.4, 0.6);
        
    return float4(color, 1.0);
}