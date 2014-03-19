#pragma once

#include "GraphicsIncludes.h"

struct ID3D11DeviceContext;

class GameObject
{
public:
	GameObject():
    mPosition(XMFLOAT3(0.0f, 0.0f, 0.0f)),
    mRotation(XMFLOAT3(0.0f, 0.0f, 0.0f)),
    mScale(XMFLOAT3(1.0f, 1.0f, 1.0f))  { }
	virtual ~GameObject() { }

	virtual void Initialize() = 0;
	virtual void Update(float dt) = 0;
  virtual void Paint(const XMMATRIX &world, const XMMATRIX &viewProj, const XMFLOAT4 &cameraPos) = 0;

  XMFLOAT3 GetPosition()
  {
    return mPosition;
  }
  void SetPosition(XMFLOAT3 position)
  {
    mPosition = position;
  }

  XMFLOAT3 GetRotation()
  {
    return mRotation;
  }
  void SetRotation(XMFLOAT3 rotation)
  {
	  mRotation = rotation;
  }

  XMFLOAT3 GetScale()
  {
    return mScale;
  }
  void SetScale(XMFLOAT3 scale)
  {
	  mScale = scale;
  }

protected:
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;
};