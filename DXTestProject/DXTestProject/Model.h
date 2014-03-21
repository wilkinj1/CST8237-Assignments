/* Model
 * Author: Justin Wilkinson
 * Purpose: A simple representation of a 3D model. I left it as vague as possible so that anyone who wants to
 * can create whatever kinds of models they want using the same interface.
 */

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
class Camera;

struct MatrixBuffer
{
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
  XMMATRIX wvp;
  XMFLOAT4 cameraPos;
};

class Model: public GameObject
{
public:
	~Model();

	void Initialize();
	void Update(float dt);
  void Paint(const XMMATRIX &world, Camera *camera, XMFLOAT3 lightPos);

  const XMFLOAT3* GetVertices();
  int GetVertexCount();

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

  XMFLOAT3 *mVertices;
  int mNumberOfVertices;
  int mSizeOfVertexDesc;

  void Paint(const XMMATRIX &world, const XMMATRIX &viewProj, const XMFLOAT4 &cameraPos) {}
};