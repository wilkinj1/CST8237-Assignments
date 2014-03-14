#pragma once

#include "Paintable.h"
#include <stack>

class MatrixStack
{
public:
  void PushMatrix(const XMMATRIX &matrix);
  const XMMATRIX& PopMatrix();

  const XMMATRIX& GetCurrentMatrix();

protected:
  XMFLOAT4X4 mCurrentMatrixValue;
  std::stack<XMFLOAT4X4> mMatrixValueStack;
};