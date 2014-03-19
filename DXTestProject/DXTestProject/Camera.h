#pragma once

#ifdef OLD_DX_SDK
#include <xnamath.h>
#else
#include <DirectXMath.h>
using namespace DirectX;
#endif

class Camera
{
public:
	Camera();
	Camera(float fov, float width, float height, XMFLOAT4 lookAtTarget, XMFLOAT4 position, XMFLOAT4 up = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	~Camera();

	float GetFOV();
	void SetFOV(float fov);

	float GetAspectRatio();
	void SetAspectRatio(float aspectRatio);

  XMFLOAT2 GetViewSize();
  void SetViewSize(float width, float height);

	XMFLOAT4 GetLookAtTarget();
	void SetLookAtTarget(XMFLOAT4 lookAtTarget);

	XMFLOAT4 GetPosition();
	void SetPosition(XMFLOAT4 position);

	XMFLOAT4 GetUpVector();
	void SetUpVector(XMFLOAT4 upVector);

	void Update(float dt);
  XMMATRIX GetViewMatrix();
  XMMATRIX GetProjectionMatrix();
	XMMATRIX GetViewProjectionMatrix();

protected:
	void CalculateViewMatrix();
	void CalculateProjectionMatrix();

	float mFOV;
	float mAspectRatio;
  XMFLOAT2 mViewSize;

	XMFLOAT4 mLookAtTarget;
	XMFLOAT4 mPosition;
	XMFLOAT4 mUpVector;

	XMFLOAT4X4 mViewMatrix;
	XMFLOAT4X4 mProjectionMatrix;
	XMFLOAT4X4 mViewProjectionMatrix;

	bool mRecalculateViewMatrix;
	bool mRecalculateProjectionMatrix;
};