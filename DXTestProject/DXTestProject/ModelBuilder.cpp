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
#include "GraphicsIncludes.h"

ModelBuilder::ModelBuilder() { }

Model* ModelBuilder::Create(MODEL_DESC & const modelDesc, SHADER_DESC & const vertexDesc, SHADER_DESC & const pixelDesc, TEXTURE_DESC & const textureDesc)
{
	Model *newModel = new Model();
	GraphicsManager *gm = GameEngine::GetInstance()->GetGraphicsManager();
  ID3D11Device *device = gm->GetGraphicsDevice();

  HRESULT result;
  newModel->mTextureView = ResourceManager::GetInstance()->LoadTexture(textureDesc.filepath);

  D3D11_SAMPLER_DESC &samplerDesc = *textureDesc.samplerDesc;

  result = device->CreateSamplerState(&samplerDesc, &newModel->mTextureSamplerState);
  assert(SUCCEEDED(result));

  D3D11_BUFFER_DESC matrixBufferDesc;
  ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
  matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
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
	InitData.pSysMem = modelDesc.modelData;

	// Create the vertex buffer.
	result = device->CreateBuffer( &bufferDesc, &InitData, &newModel->mVertexBuffer );

	assert(SUCCEEDED(result));

  newModel->mVertexShader = ResourceManager::GetInstance()->LoadVertexShaderAndInputLayout(vertexDesc, &modelDesc, &newModel->mInputLayout);
  newModel->mFragmentShader = ResourceManager::GetInstance()->LoadPixelShader(pixelDesc);

  std::vector<VPCNTDesc> &vertices = newModel->mVertices;
  for (int vertIndex = 0; vertIndex < modelDesc.vertexCount; vertIndex++)
  {
    vertices.push_back((*(modelDesc.modelData + vertIndex)));
  }

	newModel->mNumberOfVertices = modelDesc.vertexCount;
  newModel->mSizeOfVertexDesc = modelDesc.sizeOfVertexDesc;

	return newModel;
}

void ModelBuilder::Destroy(Model *model)
{
	if(model->mVertexBuffer)
	{
		model->mVertexBuffer->Release();
		model->mVertexBuffer = NULL;
	}

  ResourceManager::GetInstance()->UnloadResource<ID3D11InputLayout>(&model->mInputLayout);
  ResourceManager::GetInstance()->UnloadResource<ID3D11VertexShader>(&model->mVertexShader);
  ResourceManager::GetInstance()->UnloadResource<ID3D11PixelShader>(&model->mFragmentShader);
  ResourceManager::GetInstance()->UnloadResource<ID3D11ShaderResourceView>(&model->mTextureView);

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

  if (model->mTextureSamplerState)
  {
    model->mTextureSamplerState->Release();
    model->mTextureSamplerState = NULL;
  }
}