#pragma once
#include "GraphicsIncludes.h"
#include <vector>

class Model;

class CollisionMeshDelegate
{
public:
  virtual XMMATRIX GetWorldTransform() = 0;
};

class CollisionMesh
{
public:
  CollisionMesh();
  void Create(Model *model, CollisionMeshDelegate *collisionDelegate);
  bool CheckCollisions(CollisionMesh& otherMesh);

  const std::vector<XMFLOAT3>& GetVertices();
  XMMATRIX GetWorldTransform();

protected:
  std::vector<XMFLOAT3> mVertices;
  CollisionMeshDelegate *mDelegate;
};