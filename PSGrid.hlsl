struct PSInput
{
    float4 position : SV_Position;
    float4 color : COLOR;
    //uint primitiveID : SV_PrimitiveID;
};

float4 main(PSInput input) : SV_Target
{    
    float4 color = input.color;
    
    if (color.g)
        color.rgb = clamp(color.rgb * color.a, 0.25, 0.5);
    else
        color.rgb = clamp(color.rgb * color.a, 0.25, 1.0);
    
    return color;
}