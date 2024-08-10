struct PSInput
{
    float4 position : SV_Position;
    uint primitiveID : SV_PrimitiveID;
};

float4 main(PSInput input) : SV_Target
{
    float4 color;
    
    if (input.primitiveID % 3 == 0)
        return float4(1.0, 0.0, 0.0, 1.0);
    else if (input.primitiveID % 3 == 1)
        return float4(0.0, 1.0, 0.0, 1.0);
    else if (input.primitiveID % 3 == 2)
        return float4(0.0, 0.0, 1.0, 1.0);
    
    return color;
}