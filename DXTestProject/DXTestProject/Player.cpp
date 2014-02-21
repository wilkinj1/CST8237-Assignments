#include "stdafx.h"
#include "Player.h"
#include <d3d11.h>
#include "NewGameEngine.h"
#include <D3DX11async.h>
#include <assert.h>

#ifdef OLD_DX_SDK
#include <xnamath.h>
#else
#include <DirectXMath.h>
using namespace DirectX;
#endif

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

Player::Player():
  mVertexBuffer(NULL),
  mVertexShader(NULL),
  mPixelShader(NULL),
  mInputLayout(NULL),
  mWVPMatrixBuffer(NULL)
{
}

Player::~Player()
{
  // Do our cleanup
  if(mVertexBuffer)
  {
    mVertexBuffer->Release();
  }

  if(mVertexShader)
  {
    mVertexShader->Release();
  }

  if(mPixelShader)
  {
    mPixelShader->Release();
  }
  if(mInputLayout)
  {
    mInputLayout->Release();
  }

  if(mWVPMatrixBuffer)
  {
    mWVPMatrixBuffer->Release();
  }
}

void Player::Initialize()
{
	VertexDescription vertices[] = 
	{
		VertexDescription(/*position*/ Vector3(0.5f, 0.0f, 0.5f), /*color*/ Vector3(1.0f, 0.0f, 1.0f)),
		VertexDescription(/*position*/ Vector3(0.0f, 0.0f, 0.5f), /*color*/ Vector3(1.0f, 1.0f, 0.0f)),
		VertexDescription(/*position*/ Vector3(0.0f, 0.5f, 0.5f), /*color*/ Vector3(1.0f, 0.0f, 1.0f)),
	};

  ID3D11Device *device = NewGameEngine::GetInstance()->GetD3DDevice();
  ID3D11DeviceContext *dc = NewGameEngine::GetInstance()->GetD3DDeviceContext();

  /* Creating a description of our constant buffer.
   * This will allow us to pass our world-view-projection transformation into our shader. */
	D3D11_BUFFER_DESC worldViewProjectionMatrixBufferDesc;
	ZeroMemory(&worldViewProjectionMatrixBufferDesc, sizeof(worldViewProjectionMatrixBufferDesc));
	worldViewProjectionMatrixBufferDesc.ByteWidth = sizeof(XMMATRIX);
  worldViewProjectionMatrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	worldViewProjectionMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  HRESULT result = device->CreateBuffer(&worldViewProjectionMatrixBufferDesc, NULL, &mWVPMatrixBuffer);

  assert(SUCCEEDED(result)); // Make sure our constant buffer is created successfully.

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

  // Create our vertex buffer.
  result = device->CreateBuffer(&vertexBufferDesc, &vertexResData, &mVertexBuffer);

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
    position,
		colour
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

  /*
   * We'd normally start with an identity matrix to make sure that we get a fresh start;
   * matrix multiplication is very different from standard multiplication in that it isn't
   * commutitive (A * B != B * A). Because of this, multiplication order is VERY IMPORTANT.
   * However, since we have matrix helper functions thanks to Direct X, we'll use those.
   *
   * From here, we'll need to figure out where the player is positioned, how much they're 
   * rotated, and how much they're scaled so that we can create our world matrix.
   * The order we'll use is (rotation * translation * scale) */

  XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
  XMMATRIX rotation = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
  XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

  XMMATRIX worldMatrix = rotation * translation * scale;

  /* We need some way of getting the view and projection matrices here.
   * They will transform our geometry depending on where we're looking, and how our perspective
   * is set up. This is primarily taken care of by the Camera class. */

  XMMATRIX viewProjectionMatrix = XMMatrixIdentity();
  XMMATRIX wvp = worldMatrix * viewProjectionMatrix;

  ID3D11Device *device = NewGameEngine::GetInstance()->GetD3DDevice();
  ID3D11DeviceContext *dc = NewGameEngine::GetInstance()->GetD3DDeviceContext();
  
  // Set up our input assembler.
  dc->IASetInputLayout(mInputLayout);
  dc->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
  dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Set our shaders
  dc->VSSetShader(mVertexShader, NULL, 0);
  dc->PSSetShader(mPixelShader, NULL, 0);

  dc->UpdateSubresource(mWVPMatrixBuffer, 0, 0, &wvp, 0, 0);
  dc->VSSetConstantBuffers(0, 1, &mWVPMatrixBuffer);

  // FINALLY, WE GET TO DRAW.
  dc->Draw(3, 0);
}