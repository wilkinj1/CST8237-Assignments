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

bool CollisionMesh::CheckCollisionsGJK1(CollisionMesh& otherMesh)
{
  std::vector<XMFLOAT3> convexHull;
  bool foundOrigin = false;

  std::vector<VPCNTDesc> vertices = GetVertices();
  std::vector<VPCNTDesc> otherVertices = otherMesh.GetVertices();
  XMMATRIX otherWorld = otherMesh.GetWorldTransform();
  XMMATRIX world = GetWorldTransform();

  // Pre-multiply the model's vertices so as to avoid transforming them during comparison.
  for (int vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
  {
    XMVECTOR vertexTransform = XMLoadFloat3(&vertices[vertIndex].Position);
    XMStoreFloat3(&vertices[vertIndex].Position, XMVector3Transform(vertexTransform, world));
  }

  for (int otherVertIndex = 0; otherVertIndex < otherVertices.size(); otherVertIndex++)
  {
    XMVECTOR vertexTransform = XMLoadFloat3(&otherVertices[otherVertIndex].Position);
    XMStoreFloat3(&otherVertices[otherVertIndex].Position, XMVector3Transform(vertexTransform, otherWorld));
  }

  // Now we get to the fun part; the subtraction.
  for (int vertIndex = 0; vertIndex < vertices.size() && !foundOrigin; vertIndex++)
  {
    XMFLOAT3 vertexValue = vertices[vertIndex].Position;
    XMVECTOR vertexTransform = XMLoadFloat3(&vertexValue);

    for (int otherVertIndex = 0; otherVertIndex < otherVertices.size() && !foundOrigin; otherVertIndex++)
    {
      XMVECTOR otherVertexTransform = XMLoadFloat3(&otherVertices[otherVertIndex].Position);
      XMFLOAT3 convexHullPoint;

      XMVECTOR difference = XMVectorSubtract(vertexTransform, otherVertexTransform);
      XMStoreFloat3(&convexHullPoint, difference);
      convexHull.push_back(convexHullPoint);

      foundOrigin = XMVector3Equal(difference, XMVectorZero());
    }

    convexHull.push_back(vertexValue);
  }

  if (!foundOrigin)
  {
    XMFLOAT3 collisionLine = XMFLOAT3(0.0f, 1250.0f, 500.0f);
    printf("We ain't found shit!");
    bool collision = true;
    int intersections = 0;
    for (int hullVertexIndex = 0; hullVertexIndex < convexHull.size() && convexHull.size() > 3; hullVertexIndex += 3)
    {
      int secondIndex = (hullVertexIndex + 1) % (convexHull.size() - 1);
      int thirdIndex = (hullVertexIndex + 2) % (convexHull.size() - 1);

      XMFLOAT3 firstVert = convexHull[hullVertexIndex];
      XMFLOAT3 secondVert = convexHull[secondIndex];
      XMFLOAT3 thirdVert = convexHull[thirdIndex];
      XMFLOAT3 origin = XMFLOAT3(0.0f, 0.0f, 0.0f);

      // we need to check the normal. Calculate using cross product.
      XMVECTOR firstVector = XMVectorSet(secondVert.x - firstVert.x, secondVert.y - firstVert.y, secondVert.z - firstVert.z, 0.0f);
      XMVECTOR secondVector = XMVectorSet(thirdVert.x - secondVert.x, thirdVert.y - secondVert.y, thirdVert.z - secondVert.z, 0.0f);

      XMFLOAT3 normal;
      XMStoreFloat3(&normal, XMVector3Normalize(XMVector3Cross(firstVector, secondVector)));

      // check to ensure no parallels are detected.
      float firstDot = (normal.x * collisionLine.x) + (normal.y * collisionLine.y) + (normal.z * collisionLine.z);
      if (firstDot < 0)
      {
        float delta = -((normal.x * (origin.x - firstVert.x)) + (normal.y * (origin.y - firstVert.y)) + (normal.z * (origin.z - firstVert.y))) /
          firstDot;

        if (delta < 0)
        {
          break;
        }

        XMFLOAT3 pointToCheck = XMFLOAT3(origin.x - (collisionLine.x * delta), origin.y - (collisionLine.y * delta), origin.z * (collisionLine.z * delta));

        bool firstCheck = CheckWinding(firstVert, secondVert, pointToCheck, normal);
        bool secondCheck = CheckWinding(secondVert, thirdVert, pointToCheck, normal);
        bool thirdCheck = CheckWinding(thirdVert, firstVert, pointToCheck, normal);

        if (firstCheck && secondCheck && thirdCheck)
        {
          intersections++;
        }
        else
        {
          collision = false;
        }
      }
    }

    if ((intersections % 2) == 1)
    {
      foundOrigin = true;
    }
  }

  return foundOrigin;
}

bool CollisionMesh::CheckCollisionsGJK2(CollisionMesh& otherMesh)
{
  bool collision = false;
  std::vector<XMFLOAT3> convexHull;

  std::vector<VPCNTDesc> vertices = GetVertices();
  std::vector<VPCNTDesc> otherVertices = otherMesh.GetVertices();
  XMMATRIX otherWorld = otherMesh.GetWorldTransform();
  XMMATRIX world = GetWorldTransform();
  XMFLOAT3 origin = XMFLOAT3(0.0f, 0.0f, 0.0f);

  // Pre-multiply the model's vertices so as to avoid transforming them during comparison.
  for (int vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
  {
    XMStoreFloat3(&vertices[vertIndex].Position, XMVector3Transform(XMLoadFloat3(&vertices[vertIndex].Position), world));
    XMStoreFloat3(&vertices[vertIndex].Normal, XMVector3Transform(XMLoadFloat3(&vertices[vertIndex].Normal), world));
  }

  for (int otherVertIndex = 0; otherVertIndex < otherVertices.size(); otherVertIndex++)
  {
    XMStoreFloat3(&otherVertices[otherVertIndex].Position, XMVector3Transform(XMLoadFloat3(&otherVertices[otherVertIndex].Position), otherWorld));
    XMStoreFloat3(&otherVertices[otherVertIndex].Normal, XMVector3Transform(XMLoadFloat3(&otherVertices[otherVertIndex].Normal), otherWorld));
  }

  std::vector<VPCNTDesc> supportingVertices;
  for (int vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
  {
    VPCNTDesc &firstVertex = vertices[vertIndex];
    VPCNTDesc &secondVertex = vertices[(vertIndex + 1) % vertices.size()];

    supportingVertices.clear();
    supportingVertices = GetSupportingVertices(otherVertices, firstVertex.Normal);

    for (int supportingVertexIndex = 0; supportingVertexIndex < supportingVertices.size(); supportingVertexIndex++)
    {
      XMFLOAT3 firstFormPoint, secondFormPoint;
      XMStoreFloat3(&firstFormPoint, XMVectorSubtract(XMLoadFloat3(&supportingVertices[supportingVertexIndex].Position), XMLoadFloat3(&firstVertex.Position)));
      XMStoreFloat3(&secondFormPoint, XMVectorSubtract(XMLoadFloat3(&supportingVertices[supportingVertexIndex].Position), XMLoadFloat3(&secondVertex.Position)));

      XMFLOAT3 edgeDistValue;
      XMStoreFloat3(&edgeDistValue, XMVector3Dot(XMLoadFloat3(&firstFormPoint), XMLoadFloat3(&firstVertex.Normal)));

      float edgeDist = edgeDistValue.x;

      // project the origin onto our edge.
      XMVECTOR firstToSecondVector = XMVectorSubtract(XMLoadFloat3(&firstFormPoint), XMLoadFloat3(&secondFormPoint));
      XMVECTOR secondToFirstVector = XMVectorSubtract(XMLoadFloat3(&secondFormPoint), XMLoadFloat3(&firstFormPoint));

      XMVECTOR fTSDotVector = XMVector3Dot(firstToSecondVector, XMVectorSet(firstFormPoint.x - origin.x, firstFormPoint.y - origin.y, firstFormPoint.z - origin.z, 0.0f));
      XMVECTOR sTFDotVector = XMVector3Dot(secondToFirstVector, XMVectorSet(secondFormPoint.x - origin.x, secondFormPoint.y - origin.y, secondFormPoint.z - origin.z, 0.0f));
      XMFLOAT3 firstDotValue, secondDotValue;
      XMStoreFloat3(&firstDotValue, fTSDotVector);
      XMStoreFloat3(&secondDotValue, sTFDotVector);

      XMFLOAT3 projectedPoint;

      if (firstDotValue.x > XMConvertToRadians(90.0f))
      {
        projectedPoint = firstFormPoint;
      }
      else if (secondDotValue.x > XMConvertToRadians(90.0f))
      {
        projectedPoint = secondFormPoint;
      }
      else
      {

      }
    }

  }

  return collision;
}

int CollisionMesh::CheckWinding(XMFLOAT3 firstPoint, XMFLOAT3 secondPoint, XMFLOAT3 thirdPoint, XMFLOAT3 normal)
{
  float i = ((secondPoint.y - firstPoint.y) * (thirdPoint.z - firstPoint.z)) - ((thirdPoint.y - firstPoint.y) * (secondPoint.z - firstPoint.z));
  float j = ((secondPoint.z - firstPoint.z) * (thirdPoint.x - firstPoint.x)) - ((thirdPoint.z - firstPoint.z) * (secondPoint.x - firstPoint.x));
  float k = ((secondPoint.x - firstPoint.x) * (thirdPoint.y - firstPoint.y)) - ((thirdPoint.x - firstPoint.x) * (secondPoint.y - firstPoint.y));

  // Get the dot product. If it's negative, we're counter clockwise. Positi
  float dotProduct = (i * normal.x) + (j * normal.y) + (k * normal.z);

  if (dotProduct < 0)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

const std::vector<VPCNTDesc>& CollisionMesh::GetVertices()
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

std::vector<VPCNTDesc> CollisionMesh::GetSupportingVertices(std::vector<VPCNTDesc> vertices, XMFLOAT3 normal)
{
  std::vector<VPCNTDesc> supportingVertices;
  for (int firstIndex = 0; firstIndex < vertices.size(); firstIndex ++)
  {
    VPCNTDesc &firstVert = vertices[firstIndex];
    XMFLOAT3 &firstNormal = firstVert.Normal;

    // we need to check the normal. Calculate using cross product.
    XMVECTOR supportingNormalVector = XMLoadFloat3(&firstNormal);
    XMVECTOR normalVector = XMLoadFloat3(&normal);
    XMVECTOR dotVector = XMVector3Dot(supportingNormalVector, normalVector) / (XMVector3Length(supportingNormalVector) * XMVector3Length(normalVector)) ;
    XMFLOAT3 dotProduct;
    XMStoreFloat3(&dotProduct, dotVector);

    if (dotProduct.x >= XMConvertToRadians(33.0f))
    {
      supportingVertices.push_back(firstVert);
    }
  }

  return supportingVertices;
}

bool CollisionMesh::CheckCollisionsCustom(CollisionMesh &otherMesh)
{
  bool collision = false;
  std::vector<XMFLOAT3> convexHull;

  std::vector<VPCNTDesc> vertices = GetVertices();
  std::vector<VPCNTDesc> otherVertices = otherMesh.GetVertices();
  XMMATRIX otherWorld = otherMesh.GetWorldTransform();
  XMMATRIX world = GetWorldTransform();
  XMFLOAT3 origin = XMFLOAT3(0.0f, 0.0f, 0.0f);

  XMVECTOR inverse = XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f);
  
  XMVECTOR ourOriginDisplacement = XMVector3Transform(XMVectorSet(origin.x, origin.y, origin.z, 0.0f), world);
  XMMATRIX ourOriginTransform = XMMatrixTranslationFromVector(XMVectorMultiply(ourOriginDisplacement, inverse));

  // This is used for the purposes of moving the normals of the other object back to around (0, 0, 0).
  XMVECTOR theirOriginDisplacement = XMVector3Transform(XMVectorSet(origin.x, origin.y, origin.z, 0.0f), otherWorld);
  XMMATRIX theirOriginTransform = XMMatrixTranslationFromVector(XMVectorMultiply(theirOriginDisplacement, inverse));

  XMMATRIX ourOriginTranslatedWorld = ourOriginTransform * world;
  XMMATRIX theirOriginTranslatedWorld = otherWorld * ourOriginTransform;
  XMMATRIX theirOriginTranslatedWorldNormalAdjustment = theirOriginTransform * otherWorld;

  // Pre-multiply the model's vertices so as to avoid transforming them during comparison.
  for (int vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
  {
    XMStoreFloat3(&vertices[vertIndex].Position, XMVector3Transform(XMLoadFloat3(&vertices[vertIndex].Position), ourOriginTranslatedWorld));
    XMStoreFloat3(&vertices[vertIndex].Normal, XMVector3Transform(XMLoadFloat3(&vertices[vertIndex].Normal), ourOriginTranslatedWorld));
  }

  for (int otherVertIndex = 0; otherVertIndex < otherVertices.size(); otherVertIndex++)
  {
    XMStoreFloat3(&otherVertices[otherVertIndex].Position, XMVector3Transform(XMLoadFloat3(&otherVertices[otherVertIndex].Position), theirOriginTranslatedWorld));
    XMStoreFloat3(&otherVertices[otherVertIndex].Normal, XMVector3Transform(XMLoadFloat3(&otherVertices[otherVertIndex].Normal), theirOriginTranslatedWorldNormalAdjustment));
  }

  int potentialCollisions = 0;
  std::vector<XMFLOAT3> positions;

  // Now that the pre-multiplication is done, time to do our first-case checking: are we inside of it?
  for (int vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
  {
    bool localCollision = true;
    XMVECTOR ourVertex = XMLoadFloat3(&vertices[vertIndex].Position);
    XMVECTOR ourNormal = XMLoadFloat3(&vertices[vertIndex].Normal);

    // For each vertex in our mesh, we'll check to see if it resides inside our other mesh.
    for (int otherVertIndex = 0; otherVertIndex < otherVertices.size(); otherVertIndex++)
    {
      XMVECTOR otherVertex = XMLoadFloat3(&otherVertices[otherVertIndex].Position);
      XMVECTOR otherNormal = XMLoadFloat3(&otherVertices[otherVertIndex].Normal);

      /*XMVECTOR difference = XMVectorSubtract(otherVertex, ourVertex);
      XMFLOAT3 differenceDotValue, normalDotValue;
      XMStoreFloat3(&differenceDotValue, XMVector3Dot(difference, otherNormal));
      XMStoreFloat3(&normalDotValue, XMVector3Dot(otherNormal, ourNormal));

      if (differenceDotValue.x > 0)
      {
        localCollision = false;
      }*/

      XMVECTOR difference = XMVectorSubtract(ourVertex, otherVertex);
      XMFLOAT3 differenceDotValue, normalDotValue;
      XMVECTOR diffLength = XMVector3Length(difference);
      XMVECTOR normLength = XMVector3Length(otherNormal);
      XMVECTOR magnitude = XMVectorMultiply(diffLength, normLength);

      XMStoreFloat3(&differenceDotValue, XMVectorDivide(XMVector3Dot(difference, otherNormal), magnitude));
      // At this point, we should have the cosine of the angle.
      float angleInRads = acosf(differenceDotValue.x);
      float angleInDegs = XMConvertToDegrees(angleInRads);

      XMStoreFloat3(&normalDotValue, XMVector3Dot(ourNormal, otherNormal));

      if (angleInDegs < 90.0f)
      {
        localCollision = false;
      }
    }

    if (localCollision)
    {
      positions.push_back(vertices[vertIndex].Position);
    }
  }

  if (positions.empty())
  {
    // Time to do our second-case checking: is it inside of us?
    for (int otherVertIndex = 0; otherVertIndex < otherVertices.size(); otherVertIndex++)
    {
      bool localCollision = true;
      XMVECTOR otherVertex = XMLoadFloat3(&otherVertices[otherVertIndex].Position);
      XMVECTOR otherNormal = XMVector3Normalize(XMLoadFloat3(&otherVertices[otherVertIndex].Normal));

      // For each vertex in our mesh, we'll check to see if it resides inside our other mesh.
      for (int vertIndex = 0; vertIndex < vertices.size(); vertIndex++)
      {
        XMVECTOR ourVertex = XMLoadFloat3(&vertices[vertIndex].Position);
        XMVECTOR ourNormal = XMVector3Normalize(XMLoadFloat3(&vertices[vertIndex].Normal));

        XMVECTOR difference = XMVectorSubtract(otherVertex, ourVertex);
        XMFLOAT3 differenceDotValue, normalDotValue;
        XMVECTOR diffLength = XMVector3Length(difference);
        XMVECTOR normLength = XMVector3Length(ourNormal);
        XMVECTOR magnitude = XMVectorMultiply(diffLength, normLength);

        XMStoreFloat3(&differenceDotValue, XMVectorDivide(XMVector3Dot(difference, ourNormal), magnitude));
        // At this point, we should have the cosine of the angle.
        float angleInRads = acosf(differenceDotValue.x);
        float angleInDegs = XMConvertToDegrees(angleInRads);

        XMStoreFloat3(&normalDotValue, XMVector3Dot(ourNormal, otherNormal));

        if (angleInDegs < 90.0f)
        {
          localCollision = false;
        }
      }

      if (localCollision)
      {
        positions.push_back(otherVertices[otherVertIndex].Position);
      }
    }
  }

  return positions.size();
}
