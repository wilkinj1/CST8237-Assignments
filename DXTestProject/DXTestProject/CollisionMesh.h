#pragma once
#include "GraphicsIncludes.h"
#include <vector>

class Model;

class CollisionMesh
{
public:
  CollisionMesh();
  void Populate(Model *model, XMMATRIX worldMatrix);

protected:
  std::vector<XMFLOAT3> mVertices;

  XMFLOAT2 mLowHighX;
  XMFLOAT2 mLowHighY;
  XMFLOAT2 mLowHighZ;
};