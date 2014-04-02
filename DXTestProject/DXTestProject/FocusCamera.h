#pragma once

#include "Camera.h"

class FocusCamera: public Camera
{
public:
  FocusCamera(float fov, float width, float height, XMFLOAT4 lookAtTarget, XMFLOAT4 position, XMFLOAT4 up = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

  void RotateBy(XMFLOAT3 angle);

private:

};