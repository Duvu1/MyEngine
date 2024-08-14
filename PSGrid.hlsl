cbuffer PSConstant : register(b0)
{
    float3 viewLookAt;
}

struct PSInput
{
    float4 position : SV_Position;
    float4 color : COLOR;
    float threshold : BLENDWEIGHT;
    //uint primitiveID : SV_PrimitiveID;
};

float4 main(PSInput input) : SV_Target
{    
    float3 color = clamp(input.color.rgb * input.threshold, 0.25, 1.0);
    
    return float4(color, input.color.a);
}