struct PSInput
{
    float4 positionScreen : SV_POSITION;
};

float4 main(PSInput input) : SV_TARGET
{
    return float4(1.0, 0.5, 0.0, 1.0);
}