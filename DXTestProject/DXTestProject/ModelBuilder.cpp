#include "stdafx.h"
#include "ModelBuilder.h"
#include "Model.h"

#include "GameEngine.h"
#include "GraphicsManager.h"

#include <d3d11.h>
#include <dxgi.h>
#include <dxgitype.h>
//#include <d3dtypes.h>
#include <d3dcompiler.h>
#include <assert.h>
#include <string>

#include "GameEngine.h"
#include "GraphicsManager.h"

#include <d3dcommon.h>
#include <D3DX11async.h>

#ifdef __XNAMATH_H__
#include <xnamath.h>
#else
#include <DirectXMath.h>
using namespace DirectX;
#endif

ModelBuilder::ModelBuilder() { }

Model* ModelBuilder::create(const MODEL_DESC &modelDesc)
{
	Model *newModel = new Model();
	GraphicsManager *gm = GameEngine::getInstance()->getGraphicsManager();
  ID3D11Device *device = gm->GetGraphicsDevice();


  D3DX11_IMAGE_LOAD_INFO imageInfo;
  HRESULT result = D3DX11CreateShaderResourceViewFromFile(device, L"./IC504917.png", &imageInfo, NULL, &newModel->mTextureView, NULL);

  assert(SUCCEEDED(result));

  D3D11_SAMPLER_DESC samplerDesc;
  ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
  samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  result = device->CreateSamplerState(&samplerDesc, &newModel->mTextureSamplerState);
  assert(SUCCEEDED(result));

	D3D11_BUFFER_DESC matrixBufferDesc;
  ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
  matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  matrixBufferDesc.ByteWidth = sizeof(XMMATRIX)* 3;
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create the constant buffer for our world-view-projection matrix;
  result = device->CreateBuffer(&matrixBufferDesc, NULL, &newModel->mMatrixBuffer);

  D3D11_BUFFER_DESC lightBufferDesc;
  ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
  lightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  lightBufferDesc.ByteWidth = sizeof(XMFLOAT4);
  lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Create the constant buffer for our world-view-projection matrix;
  result = device->CreateBuffer(&lightBufferDesc, NULL, &newModel->mLightBuffer);

	assert(SUCCEEDED(result));

	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.Usage            = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth        = modelDesc.sizeOfData;
	bufferDesc.BindFlags        = D3D11_BIND_VERTEX_BUFFER;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof( InitData ) );
	InitData.pSysMem = modelDesc.pData;

	// Create the vertex buffer.
	result = device->CreateBuffer( &bufferDesc, &InitData, &newModel->mVertexBuffer );

	assert(SUCCEEDED(result));

	ID3DBlob *vertexShaderCode = NULL, *fragmentShaderCode = NULL;
	ID3DBlob *errorMessage = nullptr;

	//result = GraphicsManager::CompileShaderFromFile(L"./basicShader.fx", "BasicVertexShader", "vs_4_0", &vertexShaderCode, &errorMessage);
  result = D3DX11CompileFromFile(L"./basicShader.fx", NULL, NULL, "BasicVertexShader", "vs_4_0", 0, 0, NULL, &vertexShaderCode, &errorMessage, NULL);

	if(errorMessage != NULL)
	{
		std::string output = (char *)errorMessage->GetBufferPointer();
		errorMessage->Release();
	}

	assert(SUCCEEDED(result));

	//result = GraphicsManager::CompileShaderFromFile(L"./basicShader.fx", "BasicFragmentShader", "ps_4_0", &fragmentShaderCode, &errorMessage);
  result = D3DX11CompileFromFile(L"./basicShader.fx", NULL, NULL, "BasicFragmentShader", "ps_4_0", 0, 0, NULL, &fragmentShaderCode, &errorMessage, NULL);
	if(errorMessage != NULL)
	{
		std::string output = (char *)errorMessage->GetBufferPointer();
		errorMessage->Release();
	}

	assert(SUCCEEDED(result));


	result = device->CreateVertexShader(vertexShaderCode->GetBufferPointer(), vertexShaderCode->GetBufferSize(), NULL, &newModel->mVertexShader);

	if(FAILED(result))
	{
		vertexShaderCode->Release();
	}

	assert(SUCCEEDED(result));

	result = device->CreatePixelShader(fragmentShaderCode->GetBufferPointer(), fragmentShaderCode->GetBufferSize(), NULL, &newModel->mFragmentShader);

	if(FAILED(result))
	{
		fragmentShaderCode->Release();
	}

	assert(SUCCEEDED(result));

	result = device->CreateInputLayout(modelDesc.elementDescs, modelDesc.elementCount, vertexShaderCode->GetBufferPointer(), vertexShaderCode->GetBufferSize(), &newModel->mInputLayout);

	assert(SUCCEEDED(result));

	vertexShaderCode->Release();
	fragmentShaderCode->Release();

	newModel->mNumberOfVertices = modelDesc.vertexCount;
  newModel->mSizeOfVertexDesc = modelDesc.sizeOfVertexDesc;

	return newModel;
}

void ModelBuilder::destroy(Model *model)
{
	if(model->mVertexBuffer)
	{
		model->mVertexBuffer->Release();
		model->mVertexBuffer = NULL;
	}

	if(model->mInputLayout)
	{
		model->mInputLayout->Release();
		model->mInputLayout = NULL;
	}

	if(model->mVertexShader)
	{
		model->mVertexShader->Release();
		model->mVertexShader = NULL;
	}

	if(model->mFragmentShader)
	{
		model->mFragmentShader->Release();
		model->mFragmentShader = NULL;
	}

	if(model->mMatrixBuffer)
	{
    model->mMatrixBuffer->Release();
    model->mMatrixBuffer = NULL;
	}

  if (model->mLightBuffer)
  {
    model->mLightBuffer->Release();
    model->mLightBuffer = NULL;
  }

  if (model->mTextureView)
  {
    model->mTextureView->Release();
    model->mTextureView = NULL;
  }

  if (model->mTextureSamplerState)
  {
    model->mTextureSamplerState->Release();
    model->mTextureSamplerState = NULL;
  }
}