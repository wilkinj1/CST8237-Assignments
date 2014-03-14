cbuffer MatrixBuffer
{
	matrix world;
	matrix viewProj;
	matrix wvp;
	float4 cameraPos;
}

cbuffer LightBuffer
{
  float4 lightPosition;
}

struct VertexShaderInput
{
	float4 position : POSITION;
	float4 colour : COLOR;
	float4 normal : NORMAL;
};

struct FragmentShaderInput
{
	float4 position : SV_POSITION;
	float4 colour : COLOR0;
	float4 normal : NORMAL;
	
	float4 ambientColor : COLOR1;
	float ambientIntensity : PSIZE1;
	
	float4 diffuseColor : COLOR2;
	float4 lightVector : NORMAL2;
	float4 viewVector : NORMAL3;
};

FragmentShaderInput BasicVertexShader( VertexShaderInput input )
{
	FragmentShaderInput output;
	
	float4 lightWorldPos = mul(lightPosition, world);
	float4 worldPosition = mul(input.position, world);
	
	output.position = mul(worldPosition, viewProj);
	output.colour = input.colour;
	
	output.normal = mul(input.normal, wvp);
	output.normal = normalize(input.normal);
	
	output.ambientColor = float4(0.5f, 0.5f, 0.5f, 0.5f);
	output.ambientIntensity = 1.0f;
	output.diffuseColor = float4(1.0f, 0.25f, 0.25f, 1.0f);
	
	output.lightVector = normalize(lightWorldPos - worldPosition);
	output.viewVector = normalize(cameraPos - output.position);
	
	return output;
}

float4 BasicFragmentShader( FragmentShaderInput input ) : SV_TARGET
{
	float4 normal = normalize(input.normal);
	float4 lightVec = input.lightVector;
	float diffuseTerm = clamp( dot(normal, lightVec), 0.0f, 1.0f );
	
	return input.colour * (input.ambientColor * input.ambientIntensity) * diffuseTerm;
}