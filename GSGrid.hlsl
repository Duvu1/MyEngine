cbuffer MVP : register(b0)
{
    matrix model;
    matrix inverseTranspose; // not used in this shader
    matrix view;
    matrix projection;
}

cbuffer EyePosition : register(b1)
{
    float3 eyePosition;
    float dummy;
}

struct GSInput
{
    float4 position : POSITION;
};

struct GSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    //uint primitiveID : SV_PrimitiveID;
};

[maxvertexcount(128)]
void main(point GSInput input[1], //uint primitiveID : SV_PrimitiveID,
          inout LineStream<GSOutput> outputStream)
{        
    GSOutput output;
    
    //for (int i = -25; i < 25; i++)
    //{
    //    output.position = input[0].position;
    //    output.color = output.position;
    //    //output.primitiveID = primitiveID;
    //    
    //    output.position.xyz *= i;
    //    
    //    float distance = 10.0 / max(length(output.position.xyz - float3(0.0, 0.0, 0.0)), 1.0);
    //    output.color.a = distance;
    //    
    //    output.position = mul(output.position, view);
    //    output.position = mul(output.position, projection);
    //
    //    outputStream.Append(output);
    //}
    
    float dx[4] = { 0.5, -0.5,  0.0, 0.0 };
    float dz[4] = { 0.0,  0.0, -0.5, 0.5 };
    
    float4 center = input[0].position;
            
    for (int k = 0; k < 4; k++)
    {                
        
        output.position = center + float4(dx[k], 0.0, dz[k], 0.0);

        if (output.position.x == 0.0 || output.position.z == 0.0)
            output.color = clamp(ceil(abs(output.position)), 0.0, 1.0);
        else
            output.color = float4(0.5, 0.5, 0.5, 1.0);
            
        float distance = 1.0 / max(length(output.position.xyz - eyePosition), 1.0) * 20.0;
        output.color.a = distance;
                
        output.position = mul(output.position, view);
        output.position = mul(output.position, projection);
        
        outputStream.Append(output);
        
        if (k % 2 == 1)
            outputStream.RestartStrip();
    }    
}