struct VertexShaderInput
{
	float4 position : POSITION;
	float4 colour : COLOR;
};

struct FragmentShaderInput
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
};

FragmentShaderInput BasicVertexShader( VertexShaderInput input )
{
	FragmentShaderInput output;
	
	output.position = input.position;
	output.colour = input.colour;
	return output;
}

float4 BasicFragmentShader( FragmentShaderInput input ) : SV_TARGET
{
	return input.colour;
}