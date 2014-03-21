#include "stdafx.h"
#include "MatrixStack.h"

MatrixStack::MatrixStack()
{
  Clear();
}

void MatrixStack::PushMatrix(const XMMATRIX &matrix)
{
  XMFLOAT4X4 matrixValue;
  XMStoreFloat4x4(&matrixValue, matrix);
  mMatrixValueStack.push(matrixValue);

  XMMATRIX currentMatrix = XMLoadFloat4x4(&mCurrentMatrixValue);
  currentMatrix = matrix * currentMatrix;
  XMStoreFloat4x4(&mCurrentMatrixValue, currentMatrix);
}

const XMMATRIX& MatrixStack::PopMatrix()
{
  XMFLOAT4X4 lastMatrixValue = mMatrixValueStack.top();
  mMatrixValueStack.pop();

  XMMATRIX lastMatrix = XMLoadFloat4x4(&lastMatrixValue);
  XMVECTOR determinant = XMMatrixDeterminant(lastMatrix);

  XMMATRIX inverseMatrix = XMMatrixInverse(&determinant, lastMatrix);
  XMMATRIX currentMatrix = XMLoadFloat4x4(&mCurrentMatrixValue);
  currentMatrix = inverseMatrix * currentMatrix;
  XMStoreFloat4x4(&mCurrentMatrixValue, currentMatrix);

  return lastMatrix;
}

XMMATRIX MatrixStack::GetCurrentMatrix()
{
  return XMLoadFloat4x4(&mCurrentMatrixValue);
}

void MatrixStack::Clear()
{
  XMStoreFloat4x4(&mCurrentMatrixValue, XMMatrixIdentity());
  while (!mMatrixValueStack.empty())
  {
    mMatrixValueStack.pop();
  }
}