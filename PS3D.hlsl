Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

float4 main(PSInput input) : SV_Target
{
    float4 color = float4(input.color, 1.0);
        
    return color;
}