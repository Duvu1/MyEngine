Texture2D baseColorTexture0 : register(t0);
Texture2D baseColorTexture1 : register(t1);
SamplerState baseColorSampler : register(s0);

cbuffer PSConstantBuffer : register(b0)
{
    float threshold;
    float dummy[3];
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    float4 color;

    color = lerp(baseColorTexture0.Sample(baseColorSampler, input.uv),
                 baseColorTexture1.Sample(baseColorSampler, input.uv),
                 threshold);
    
    return color;
}