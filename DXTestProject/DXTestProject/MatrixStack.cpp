#include "stdafx.h"
#include "MatrixStack.h"

void MatrixStack::PushMatrix(const XMMATRIX &matrix)
{
  XMFLOAT4X4 matrixValue;
  XMStoreFloat4x4(&matrixValue, matrix);
  mMatrixValueStack.push(matrixValue);

  XMMATRIX currentMatrix = XMLoadFloat4x4(&mCurrentMatrixValue);
  currentMatrix *= matrix;
  XMStoreFloat4x4(&mCurrentMatrixValue, currentMatrix);
}

const XMMATRIX& MatrixStack::PopMatrix()
{
  XMFLOAT4X4 lastMatrixValue = mMatrixValueStack.top();
  XMMATRIX lastMatrix = XMLoadFloat4x4(&lastMatrixValue);
  XMVECTOR determinant = XMMatrixDeterminant(lastMatrix);

  XMMATRIX inverseMatrix = XMMatrixInverse(&determinant, lastMatrix);
  XMMATRIX currentMatrix = XMLoadFloat4x4(&mCurrentMatrixValue);
  currentMatrix *= inverseMatrix;
  XMStoreFloat4x4(&mCurrentMatrixValue, currentMatrix);

  return lastMatrix;
}

const XMMATRIX& MatrixStack::GetCurrentMatrix()
{
  return XMLoadFloat4x4(&mCurrentMatrixValue);
}