#pragma once

#include "Camera.h"

class FocusCamera: public Camera
{
public:
  FocusCamera(float fov, float width, float height, XMFLOAT4 lookAtTarget, XMFLOAT4 position, XMFLOAT4 up = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

  /**
   * Rotate the camera by a given amount on each axis.
   * @param angle The angle in radians that we want to rotate the camera by.
   */
  void RotateBy(XMFLOAT3 angle);

private:

};