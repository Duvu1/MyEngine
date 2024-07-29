Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float4 color = input.color;
    
    if (color.a == 0.0)
        color = baseColorTexture.Sample(baseColorSampler, input.uv);
        
    return color;
}