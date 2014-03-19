#pragma once
#include "GraphicsIncludes.h"
#include "GameObject.h"

struct ID3D11Buffer;
struct ID3D11DeviceContext;
struct ID3D11Device;
struct ID3D11InputLayout;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

struct ID3D11VertexShader;
struct ID3D11PixelShader;

class Model: public GameObject
{
public:
	~Model();

	void Initialize();
	void Update(float dt);
  void Paint(const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &proj, const XMFLOAT4 &cameraPos, XMFLOAT4 lightPos);

protected:
  friend class ModelBuilder;
  Model();

	ID3D11Buffer *mMatrixBuffer;
  ID3D11Buffer *mLightBuffer;

	ID3D11Buffer *mVertexBuffer;
	ID3D11InputLayout *mInputLayout;

	ID3D11VertexShader *mVertexShader;
	ID3D11PixelShader *mFragmentShader;

  ID3D11ShaderResourceView *mTextureView;
  ID3D11SamplerState *mTextureSamplerState;

  int mNumberOfVertices;
  int mSizeOfVertexDesc;

  void Paint(const XMMATRIX &world, const XMMATRIX &viewProj, const XMFLOAT4 &cameraPos) {}
};