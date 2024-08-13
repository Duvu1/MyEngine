cbuffer MVP : register(b0)
{
    matrix model;
    matrix inverseTranspose; // not used in this shader
    matrix view;
    matrix projection;
}

struct GSInput
{
    float4 position : SV_Position;
};

struct GSOutput
{
    float4 position : SV_Position;
    float4 color : COLOR;
    //uint primitiveID : SV_PrimitiveID;
};

[maxvertexcount(100)]
void main(point GSInput input[1], //uint primitiveID : SV_PrimitiveID,
          inout LineStream<GSOutput> outputStream)
{
    GSOutput output;
        
    for (int i = -50; i < 50; i++)
    {
        float offset = i;
        
        output.position = input[0].position;
        output.color = output.position;
        //output.primitiveID = primitiveID;
        
        output.position.xyz *= offset;
        
        output.position = mul(output.position, view);
        output.position = mul(output.position, projection);

        outputStream.Append(output);
    }
}