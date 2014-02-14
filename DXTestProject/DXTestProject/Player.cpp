#include "stdafx.h"
#include "Player.h"
#include <d3d11.h>
#include "NewGameEngine.h"
#include <D3DX11async.h>
#include <assert.h>

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3(float x, float y, float z):
		x(x), y(y), z(z) { }
};

struct VertexDescription
{
	Vector3 position;
	Vector3 color;

	VertexDescription(Vector3 pos, Vector3 col):
		position(pos.x, pos.y, pos.z),
		color(col.x, col.y, col.z) { }
};

Player::~Player()
{

}

void Player::Initialize()
{
	VertexDescription vertices[] = 
	{
    VertexDescription(/*position*/ Vector3(0.5f, 0.0f, 0.5f), /*color*/ Vector3(1.0f, 0.0f, 1.0f)),
		VertexDescription(/*position*/ Vector3(0.0f, 0.0f, 0.5f), /*color*/ Vector3(1.0f, 1.0f, 0.0f)),
		VertexDescription(/*position*/ Vector3(0.0f, 0.5f, 0.5f), /*color*/ Vector3(1.0f, 0.0f, 1.0f)),
	};

	// Creating a description of our buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// Creating a resource that will contain our buffer's information.
	D3D11_SUBRESOURCE_DATA vertexResData;
	ZeroMemory(&vertexResData, sizeof(vertexResData));
	vertexResData.pSysMem = vertices;
	vertexResData.SysMemPitch = 0;
	vertexResData.SysMemSlicePitch = 0;

  ID3D11Device *device = NewGameEngine::GetInstance()->GetD3DDevice();
  ID3D11DeviceContext *dc = NewGameEngine::GetInstance()->GetD3DDeviceContext();

  // Create our vertex buffer.
  HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexResData, &mVertexBuffer);

  assert(SUCCEEDED(result)); // Make sure our vertex buffer is created successfully.

  // Compiling our vertex shader code
  ID3DBlob *vertexShaderCode;
  ID3DBlob *errorMessage;
  result = D3DX11CompileFromFile(L"./basicShader.fx",
    NULL, NULL, "BasicVertexShader", "vs_4_0", 0, 0, NULL,
    &vertexShaderCode, &errorMessage, NULL);

  assert(SUCCEEDED(result)); // Make sure our vertex shader is compiled successfully.

  // Compiling our pixel/fragment shader code
  ID3DBlob *fragmentShaderCode;
  result = D3DX11CompileFromFile(L"./basicShader.fx",
    NULL, NULL, "BasicFragmentShader", "ps_4_0", 0, 0, NULL,
    &fragmentShaderCode, &errorMessage, NULL);

  assert(SUCCEEDED(result)); // Make sure our pixel/fragment shader is compiled successfully.

  result = device->CreateVertexShader(vertexShaderCode->GetBufferPointer(),
    vertexShaderCode->GetBufferSize(), NULL, &mVertexShader);

  assert(SUCCEEDED(result)); // Make sure our vertex shader is created successfully.

  result = device->CreatePixelShader(fragmentShaderCode->GetBufferPointer(),
    fragmentShaderCode->GetBufferSize(), NULL, &mPixelShader);

  assert(SUCCEEDED(result)); // Make sure our fragment shader is compiled successfully.

  /* Create our input layout. For each attribute of our vertex, we make
   * an element description. */ 
  D3D11_INPUT_ELEMENT_DESC position;
  ZeroMemory(&position, sizeof(position));
  position.SemanticName = "POSITION"; // The name we use to map the element to the shader.
  position.SemanticIndex = 0;
  position.Format = DXGI_FORMAT_R32G32B32_FLOAT; // The format of our element (three 32-bit floating point values)
  position.AlignedByteOffset = 0; // How many bytes in we'll find the start of this element.
  position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  position.InputSlot = 0;

  D3D11_INPUT_ELEMENT_DESC colour;
  ZeroMemory(&colour, sizeof(colour));
  colour.SemanticName = "COLOR";
  colour.SemanticIndex = 0;
  colour.Format = DXGI_FORMAT_R32G32B32_FLOAT;
  colour.AlignedByteOffset = sizeof(Vector3);
  colour.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  colour.InputSlot = 0;

  D3D11_INPUT_ELEMENT_DESC elementDescriptions[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	
  };

  result = device->CreateInputLayout(elementDescriptions,
    ARRAYSIZE(elementDescriptions), vertexShaderCode->GetBufferPointer(),
    vertexShaderCode->GetBufferSize(), &mInputLayout);

  assert(SUCCEEDED(result)); // Make sure our input layout was created successfully.

  // Clean up the compiled code objects
  vertexShaderCode->Release();
  fragmentShaderCode->Release();
}

void Player::Update(float dt)
{

}

void Player::Render()
{
  unsigned int stride = sizeof(VertexDescription);
  unsigned int offset = 0;

  // We need some way of getting the view and projection matrices here.
  // Also, we'll need to figure out where the player is positioned, how much they're 
  // rotated, and how much they're scaled so that we can create our world matrix.

  // From there, it's just r * t * s.

  ID3D11Device *device = NewGameEngine::GetInstance()->GetD3DDevice();
  ID3D11DeviceContext *dc = NewGameEngine::GetInstance()->GetD3DDeviceContext();
  
  // Set up our input assembler.
  dc->IASetInputLayout(mInputLayout);
  dc->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
  dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Set our shaders
  dc->VSSetShader(mVertexShader, NULL, 0);
  dc->PSSetShader(mPixelShader, NULL, 0);

  // FINALLY, WE GET TO DRAW.
  dc->Draw(3, 0);
}