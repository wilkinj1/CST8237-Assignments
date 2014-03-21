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
  float3 ambientColor;
  float3 lightColor;
}

/* A structure that represents our vertex. */
struct VertexShaderInput
{
	float4 position : POSITION0;
	float3 colour : COLOR0;
	float3 normal : NORMAL0;
	float2 texUV : TEXCOORD0;
};

/* A structure that represents the processed vertex, plus any
 * additional information we want passed to the pixel shader. */
struct FragmentShaderInput
{
	float4 position : SV_POSITION;
	float3 colour : COLOR0;
	float3 normal : NORMAL0;
	float2 texUV : TEXCOORD0;
	
	float3 lightVector : NORMAL2;
	float3 viewVector : NORMAL3;
};

FragmentShaderInput BasicVertexShader( VertexShaderInput input )
{
	FragmentShaderInput output;
	
	/* Since we need to know where the vertex is in the world, we multiply
     * it by the given world matrix. However, we want to use the 'worldPosition'
     * for calculations that don't require the view or projection matrices, so
     * we multiply them in separately later. */
	float4 worldPosition = mul(input.position, world);
	
	output.position = mul(worldPosition, view);
	output.position = mul(output.position, proj);
	output.colour = input.colour;
	
	output.texUV = input.texUV;
	
	output.normal = (mul(input.normal, (float3x3)world));
	
	output.lightVector = lightPosition - (float3)worldPosition;
	output.viewVector = normalize(cameraPos - (float3)worldPosition);
	
	return output;
}

float4 BasicFragmentShader( FragmentShaderInput input ) : SV_TARGET
{
	float3 ambientColor = float3(0.2f, 0.2f, 0.2f);
	//output.ambientIntensity = 1.0f;
	float3 colour = float3(0.7f, 0.7f, 0.7f);

	float3 normal = normalize(input.normal);
	float3 lightVec = normalize(input.lightVector);
	
	/* The diffuseTerm is the angle between where the normal, and the vector from
	 * where the light is to the pixel being lit; if the angle is too small (or
	 * negative), then the object isn't being lit. */
	float diffuseTerm = clamp( dot(normal, lightVec), 0.0f, 1.0f );
	
	/* We get the texture color from the texture based on the UV coordinate. */
	float4 textureColor = mainTexture.Sample(textureState, input.texUV);
	float3 finalColor = input.colour * (ambientColor + colour * diffuseTerm);
	return textureColor * float4(finalColor, 1.0f);
}