#include "stdafx.h"
#include "Obstacle.h"
#include <assert.h>
#include <string>

#include "GameEngine.h"
#include "GraphicsManager.h"

#include <d3dcommon.h>
#include "Model.h"
#include "BasicShaderModelBuilder.h"
#include "ModelUtils.h"
#include "MatrixStack.h"
#include "Camera.h"

Obstacle::Obstacle(): 
GameObject(),
  mObstacleModel(NULL)
{
}

Obstacle::~Obstacle()
{
  if(mObstacleModel)
  {
    ModelUtils::DestroyModel(mObstacleModel);
    mObstacleModel = NULL;
  }
}

void Obstacle::Initialize()
{
  mObstacleModel = ModelUtils::CreateCubeModelPCNT();
}

void Obstacle::Update(float dt)
{
  mObstacleModel->Update(dt);
}

void Obstacle::Paint(const XMMATRIX &world, Camera *camera, XMFLOAT3 lightPos)
{
  /* Since we're now leveraging the matrix stack, we can just push our transformation matrices right into it.
   * Remember that we push them in the OPPOSITE direction we expect them multiplied; it's LIFO, so we multiply
   * starting at the end, and moving towards the first element we pushed in. */
  MatrixStack matrix;
  matrix.PushMatrix(world);
  matrix.PushMatrix(GetWorldTransform());

  mObstacleModel->Paint(matrix.GetCurrentMatrix(), camera, lightPos);
}

XMMATRIX Obstacle::GetWorldTransform()
{
  MatrixStack matrix;

  matrix.PushMatrix(XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z));
  matrix.PushMatrix(XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z));
  matrix.PushMatrix(XMMatrixScaling(mScale.x, mScale.y, mScale.z));

  return matrix.GetCurrentMatrix();
}

Model* Obstacle::GetModel()
{
  return mObstacleModel;
}

void Obstacle::CollisionOccurred(CollisionMeshDelegate *otherObject)
{
  printf("Obstacle collided with object!\n");
}