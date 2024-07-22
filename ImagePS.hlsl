Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

cbuffer PSConstantBuffer : register(b0)
{
    bool textureOnOff;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float4 color = input.color;
    
    if (textureOnOff == true)
        color = baseColorTexture.Sample(baseColorSampler, input.uv);
    
    return color;
}