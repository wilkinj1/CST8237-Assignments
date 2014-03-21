#include "stdafx.h"
#include "CollisionMesh.h"
#include "Model.h"

CollisionMesh::CollisionMesh()
{

}

void CollisionMesh::Populate(Model *model, XMMATRIX worldMatrix)
{
  mLowHighX.x = -D3D11_FLOAT32_MAX;
  mLowHighX.y = D3D11_FLOAT32_MAX;

  mLowHighY.x = -D3D11_FLOAT32_MAX;
  mLowHighY.y = D3D11_FLOAT32_MAX;

  mLowHighZ.x = -D3D11_FLOAT32_MAX;
  mLowHighZ.y = D3D11_FLOAT32_MAX;
}