struct PSInput
{
    float4 position : SV_Position;
    float4 color : COLOR;
    //uint primitiveID : SV_PrimitiveID;
};

float4 main(PSInput input) : SV_Target
{
    return input.color;
}