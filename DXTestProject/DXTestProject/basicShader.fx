Texture2D mainTexture;
SamplerState textureState;

cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix proj;
	matrix wvp;
	float3 cameraPos;
}

cbuffer LightBuffer
{
  float3 lightPosition;
}

struct VertexShaderInput
{
	float4 position : POSITION0;
	float3 colour : COLOR0;
	float3 normal : NORMAL0;
	float2 texUV : TEXCOORD0;
};

struct FragmentShaderInput
{
	float4 position : SV_POSITION;
	float3 colour : COLOR0;
	float3 normal : NORMAL0;
	float2 texUV : TEXCOORD0;
	
	/*float3 ambientColor : COLOR1;
	float ambientIntensity : PSIZE1;
	
	float3 diffuseColor : COLOR2;*/
	float3 lightVector : NORMAL2;
	float3 viewVector : NORMAL3;
};

FragmentShaderInput BasicVertexShader( VertexShaderInput input )
{
	FragmentShaderInput output;
	
	float4 lightWorldPos = float4( lightPosition, 0.0f );
	float4 worldPosition = mul(input.position, world);
	
	output.position = mul(worldPosition, view);
	output.position = mul(output.position, proj);
	output.colour = input.colour;
	output.texUV = input.texUV;
	
	output.normal = (mul(input.normal, (float3x3)world));
	
	output.lightVector = (float3)(lightWorldPos - worldPosition);
	output.viewVector = normalize(cameraPos - (float3)worldPosition);
	
	return output;
}

float4 BasicFragmentShader( FragmentShaderInput input ) : SV_TARGET
{
	float3 ambientColor = float3(0.2f, 0.2f, 0.2f);
	//output.ambientIntensity = 1.0f;
	float3 color = float3(0.7f, 0.7f, 0.7f);

	float3 normal = normalize(input.normal);
	float3 lightVec = normalize(input.lightVector);
	float diffuseTerm = clamp( dot(normal, lightVec), 0.0f, 1.0f );
	
	float4 textureColor = mainTexture.Sample(textureState, input.texUV);
	float3 finalColor = (ambientColor + color * diffuseTerm);
	return textureColor * float4(finalColor, 1.0f);
}