struct PSInput
{
    float4 positionScreen : SV_POSITION;
    float3 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET
{
    return float4(input.color, 1.0);
}