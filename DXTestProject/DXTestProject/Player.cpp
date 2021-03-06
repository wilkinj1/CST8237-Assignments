#include "stdafx.h"
#include "Player.h"
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

#include "Animation.h"

Player::Player(): 
GameObject(),
  mPlayerModel(NULL)
{
}

Player::~Player()
{
  if(mPlayerModel)
  {
    ModelUtils::DestroyModel(mPlayerModel);
    mPlayerModel = NULL;
  }
}

void Player::Initialize()
{
  //mPlayerModel = ModelUtils::CreateCubeModelPCNTWithTextureFilename(L"./200px-A_dragon.PNG");
  mPlayerModel = ModelUtils::CreateCubeModelPCNTWithAnimationFilename(L"./trogdor.anim");
  Animation *anim = mPlayerModel->GetAnimation();
  anim->Play("walk");
}

void Player::Update(float dt)
{
  mPlayerModel->Update(dt);
}

void Player::Paint(const XMMATRIX &world, Camera *camera, XMFLOAT3 lightPos)
{
  /* Since we're now leveraging the matrix stack, we can just push our transformation matrices right into it.
   * Remember that we push them in the OPPOSITE direction we expect them multiplied; it's LIFO, so we multiply
   * starting at the end, and moving towards the first element we pushed in. */
  MatrixStack matrix;
  matrix.PushMatrix(world);
  matrix.PushMatrix(GetWorldTransform());

  mPlayerModel->Paint(matrix.GetCurrentMatrix(), camera, lightPos);
}

XMMATRIX Player::GetWorldTransform()
{
  MatrixStack matrix;

  matrix.PushMatrix(XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z));
  matrix.PushMatrix(XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z));
  matrix.PushMatrix(XMMatrixScaling(mScale.x, mScale.y, mScale.z));

  return matrix.GetCurrentMatrix();
}

Model* Player::GetModel()
{
  return mPlayerModel;
}

void Player::CollisionOccurred(CollisionMeshDelegate *otherObject)
{
  printf("Player collided with object!\n");
}