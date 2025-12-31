struct Input {
    float4 pos : POSITION0;
    float4 normal : NORMAL0;
    float2 uv : TEXCOORD0;
    float4 instanceMat0 : INSTANCEMAT0;
    float4 instanceMat1 : INSTANCEMAT1;
    float4 instanceMat2 : INSTANCEMAT2;
    float4 instanceMat3 : INSTANCEMAT3;
};

cbuffer ModelData : register(b0) {
    float4x4 Model;
    bool isInstance;
};
cbuffer CameraData : register(b1) {
    float4x4 View;
    float4x4 Projection;
};

struct Output {
    float4 pos : SV_POSITION;
    float4 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

float4x4 float4to4x4(float4 a, float4 b, float4 c, float4 d)
{
    return float4x4(a, b, c, d);

};

Output main(Input input) {
	Output output = (Output)0;
    
    float4x4 FinalModel;
    
    if (isInstance)
    {
        FinalModel = float4to4x4(input.instanceMat0,input.instanceMat1,input.instanceMat2,input.instanceMat3);
    }
    else
    {
        FinalModel = Model;
    }
    
    output.pos = mul(input.pos, FinalModel);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
    output.normal = mul(input.normal, FinalModel);
    output.uv = input.uv; 

	return output;
}