#include "stdafx.h"
#include "CollisionMesh.h"
#include "Model.h"

CollisionMesh::CollisionMesh()
{

}

void CollisionMesh::Create(Model *model, CollisionMeshDelegate *collisionDelegate)
{
  mVertices = model->GetVertices();
  mDelegate = collisionDelegate;
}

bool CollisionMesh::CheckCollisions(CollisionMesh& otherMesh)
{
  std::vector<XMFLOAT3> convexHull;

  std::vector<XMFLOAT3> vertices = GetVertices();
  std::vector<XMFLOAT3> otherVertices = otherMesh.GetVertices();
  XMMATRIX otherWorld = otherMesh.GetWorldTransform();
  XMMATRIX world = GetWorldTransform();

  // Pre-multiply the model's vertices so as to avoid transforming them during comparison.
  for (int vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
  {
    XMVECTOR vertexTransform = XMLoadFloat3(&vertices[vertIndex]);
    XMStoreFloat3(&vertices[vertIndex], XMVector3Transform(vertexTransform, world));
  }

  for (int otherVertIndex = 0; otherVertIndex < otherVertices.size(); otherVertIndex++)
  {
    XMVECTOR vertexTransform = XMLoadFloat3(&otherVertices[otherVertIndex]);
    XMStoreFloat3(&otherVertices[otherVertIndex], XMVector3Transform(vertexTransform, otherWorld));
  }

  bool foundOrigin = false;

  // Now we get to the fun part; the subtraction.
  for (int vertIndex = 0; vertIndex < vertices.size() && !foundOrigin; vertIndex++)
  {
    XMVECTOR vertexTransform = XMLoadFloat3(&vertices[vertIndex]);

    for (int otherVertIndex = 0; otherVertIndex < otherVertices.size() && !foundOrigin; otherVertIndex++)
    {
      XMVECTOR otherVertexTransform = XMLoadFloat3(&otherVertices[otherVertIndex]);
      XMFLOAT3 convexHullPoint;

      XMVECTOR difference = XMVectorSubtract(vertexTransform, otherVertexTransform);
      XMStoreFloat3(&convexHullPoint, difference);
      convexHull.push_back(convexHullPoint);

      foundOrigin = XMVector3Equal(difference, XMVectorZero());
    }
  }

  if (!foundOrigin)
  {
    printf("We ain't found shit!");
  }

  return foundOrigin;
}

const std::vector<XMFLOAT3>& CollisionMesh::GetVertices()
{
  return mVertices;
}

XMMATRIX CollisionMesh::GetWorldTransform()
{
  XMMATRIX world = XMMatrixIdentity();
  if (mDelegate)
  {
    world = mDelegate->GetWorldTransform();
  }

  return world;
}