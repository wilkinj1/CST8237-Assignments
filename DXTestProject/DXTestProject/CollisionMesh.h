#pragma once
#include "GraphicsIncludes.h"
#include <vector>
#include "Model.h"

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
  bool CheckCollisionsGJK1(CollisionMesh& otherMesh);
  bool CheckCollisionsGJK2(CollisionMesh& otherMesh);
  bool CheckCollisionsCustom(CollisionMesh &otherMesh);

  const std::vector<VPCNTDesc>& GetVertices();
  XMMATRIX GetWorldTransform();

protected:
  std::vector<VPCNTDesc> mVertices;
  CollisionMeshDelegate *mDelegate;

  int CheckWinding(XMFLOAT3 firstPoint, XMFLOAT3 secondPoint, XMFLOAT3 thirdPoint, XMFLOAT3 normal);

  std::vector<VPCNTDesc> GetSupportingVertices(std::vector<VPCNTDesc> vertices, XMFLOAT3 normal);
};