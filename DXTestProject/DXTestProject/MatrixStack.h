#pragma once

#include "GraphicsIncludes.h"
#include <stack>

class MatrixStack
{
public:
  void PushMatrix(const XMMATRIX &matrix);
  const XMMATRIX& PopMatrix();
  void Clear();

  const XMMATRIX& GetCurrentMatrix();

protected:
  XMFLOAT4X4 mCurrentMatrixValue;
  std::stack<XMFLOAT4X4> mMatrixValueStack;
};