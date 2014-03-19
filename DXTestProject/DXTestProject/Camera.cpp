#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	SetFOV(0.0f);
	SetAspectRatio(0.0f);
	SetLookAtTarget(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	SetPosition(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	SetUpVector(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}

Camera::Camera(float fov, float width, float height, XMFLOAT4 lookAtTarget, XMFLOAT4 position, XMFLOAT4 up)
{
	SetFOV(fov);
  SetViewSize(width, height);
	SetLookAtTarget(lookAtTarget);
	SetPosition(position);
	SetUpVector(up);
}

Camera::~Camera() {} 

float Camera::GetFOV()
{
	return mFOV;
}

void Camera::SetFOV(float fov)
{
	mFOV = fov;
	mRecalculateProjectionMatrix = true;
}

XMFLOAT2 Camera::GetViewSize()
{
  return mViewSize;
}

void Camera::SetViewSize(float width, float height)
{
  mViewSize = XMFLOAT2(width, height);
  SetAspectRatio(width / height);
}

float Camera::GetAspectRatio()
{
	return mAspectRatio;
}

void Camera::SetAspectRatio(float aspectRatio)
{
	mAspectRatio = aspectRatio;
	mRecalculateProjectionMatrix = true;
}

XMFLOAT4 Camera::GetLookAtTarget()
{
	return mLookAtTarget;
}

void Camera::SetLookAtTarget(XMFLOAT4 lookAtTarget)
{
	mLookAtTarget = lookAtTarget;
	mRecalculateViewMatrix = true;
}

XMFLOAT4 Camera::GetPosition()
{
	return mPosition;
}

void Camera::SetPosition(XMFLOAT4 position)
{
	mPosition = position;
	mRecalculateViewMatrix = true;
}

XMFLOAT4 Camera::GetUpVector()
{
	return mUpVector;
}

void Camera::SetUpVector(XMFLOAT4 upVector)
{
	mUpVector = upVector;
	mRecalculateViewMatrix = true;
}

void Camera::Update(float dt)
{
	bool recalculateViewProjectionMatrix = mRecalculateViewMatrix || mRecalculateProjectionMatrix;
	if(mRecalculateViewMatrix)
	{
		CalculateViewMatrix();
	}

	if(mRecalculateProjectionMatrix)
	{
		CalculateProjectionMatrix();
	}

	if(recalculateViewProjectionMatrix)
	{
		XMMATRIX view = XMLoadFloat4x4(&mViewMatrix);
		XMMATRIX proj = XMLoadFloat4x4(&mProjectionMatrix);
		XMMATRIX viewProj = view * proj;
		XMStoreFloat4x4(&mViewProjectionMatrix, viewProj);
	}
}

XMMATRIX Camera::GetViewMatrix()
{
  return XMLoadFloat4x4(&mViewMatrix);
}

XMMATRIX Camera::GetProjectionMatrix()
{
  return XMLoadFloat4x4(&mProjectionMatrix);
}

XMMATRIX Camera::GetViewProjectionMatrix()
{
	return XMLoadFloat4x4(&mViewProjectionMatrix);
}

void Camera::CalculateViewMatrix()
{
	if(mRecalculateViewMatrix)
	{
		XMFLOAT4 targetDifference(mLookAtTarget.x - mPosition.x, mLookAtTarget.y - mPosition.y, mLookAtTarget.z - mPosition.z, 1.0f);

		XMVECTOR eyeVector = XMLoadFloat4(&mPosition);

		XMVECTOR targetVector = XMLoadFloat4(&targetDifference);
		targetVector = XMVector4Normalize(targetVector);

		XMVECTOR upVector = XMLoadFloat4(&mUpVector);

		XMMATRIX viewMatrix = XMMatrixLookAtLH(eyeVector, targetVector, upVector);
		XMStoreFloat4x4(&mViewMatrix, viewMatrix);

		mRecalculateViewMatrix = false;
	}
}

void Camera::CalculateProjectionMatrix()
{
	if(mRecalculateProjectionMatrix)
	{
		XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, 0.1f, 100.0f);
		XMStoreFloat4x4(&mProjectionMatrix, projectionMatrix);
		mRecalculateProjectionMatrix = false;
	}
}