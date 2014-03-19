#include "stdafx.h"
#include "Model.h"
#include <d3d11.h>
#include <dxgi.h>
#include <dxgitype.h>
#include <d3dcompiler.h>
#include <assert.h>
#include <string>

#include "GameEngine.h"
#include "GraphicsManager.h"

#include <d3dcommon.h>

struct MatrixBuffer
{
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
  XMMATRIX wvp;
  XMFLOAT4 cameraPos;
};

Model::Model(): 
GameObject(),
	mVertexBuffer(NULL),
	mInputLayout(NULL),
	mVertexShader(NULL),
	mFragmentShader(NULL),
	mMatrixBuffer(NULL),
  mLightBuffer(NULL)
{
}

Model::~Model()
{
}

void Model::Initialize()
{
}

void Model::Update(float dt)
{
}

void Model::Paint(const XMMATRIX &world, const XMMATRIX& view, const XMMATRIX &proj, const XMFLOAT4 &cameraPos, XMFLOAT4 lightPos)
{
  unsigned int stride = mSizeOfVertexDesc;
	unsigned int offset = 0;

  MatrixBuffer buffer;
  buffer.world = XMMatrixTranspose(world);
  buffer.view = XMMatrixTranspose(view);
  buffer.proj = XMMatrixTranspose(proj);
  buffer.wvp = XMMatrixTranspose(world * view * proj);
  buffer.cameraPos = cameraPos;

  ID3D11DeviceContext *dc = GameEngine::getInstance()->getGraphicsManager()->GetGraphicsDeviceContext();

	dc->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	dc->IASetInputLayout(mInputLayout);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->VSSetShader(mVertexShader, 0, 0);
	dc->PSSetShader(mFragmentShader, 0, 0);
  dc->PSSetShaderResources(0, 1, &mTextureView);
  dc->PSSetSamplers(0, 1, &mTextureSamplerState);

  dc->UpdateSubresource(mMatrixBuffer, 0, 0, &buffer, 0, 0);
  dc->VSSetConstantBuffers(0, 1, &mMatrixBuffer);

  dc->UpdateSubresource(mLightBuffer, 0, 0, &lightPos, 0, 0);
  dc->VSSetConstantBuffers(1, 1, &mLightBuffer);

	dc->Draw(mNumberOfVertices, 0);
}