cbuffer MVP : register(b0)
{
    matrix model;
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
    uint primitiveID : SV_PrimitiveID;
};

[maxvertexcount(100)]
void main(point GSInput input[1], uint primitiveID : SV_PrimitiveID,
          inout LineStream<GSOutput> outputStream)
{
    GSOutput output;
    
    //output.position = input[0].position;
    
    for (int i = -50; i < 50; i++)
    {
        float offset = i * 10;
        output.position = input[0].position * float4(i, i, i, 1.0);
        output.primitiveID = primitiveID;
        
        output.position = mul(output.position, view);
        output.position = mul(output.position, projection);

        outputStream.Append(output);
    }
}