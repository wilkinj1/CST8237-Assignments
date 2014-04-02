#include "stdafx.h"
#include "FocusCamera.h"

FocusCamera::FocusCamera(float fov, float width, float height, XMFLOAT4 lookAtTarget, XMFLOAT4 position, XMFLOAT4 up):
Camera(fov, width, height, lookAtTarget, position, up)
{

}

void FocusCamera::RotateBy(XMFLOAT3 angle)
{
  XMMATRIX rotation = XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
  XMVECTOR position = XMLoadFloat4(&mPosition);
  XMVECTOR lookAtPosition = XMLoadFloat4(&mLookAtTarget);

  XMMATRIX translation = XMMatrixTranslationFromVector(XMVectorSubtract(position, lookAtPosition));

  XMMATRIX transform = translation * rotation;
  XMVECTOR lookAtTargetPos = XMLoadFloat4(&mLookAtTarget);
  XMVECTOR newPosition = XMVector4Transform(lookAtTargetPos, transform);

  XMFLOAT4 newPositionValue;
  XMStoreFloat4(&newPositionValue, newPosition);

  SetPosition(newPositionValue);
}