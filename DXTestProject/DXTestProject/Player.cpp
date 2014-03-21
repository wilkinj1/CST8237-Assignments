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

Player::Player(): 
GameObject(),
  mPlayerModel(NULL),
  mSatelliteModel(NULL)
{
}

Player::~Player()
{
  if(mPlayerModel)
  {
    BasicShaderModelBuilder::Destroy(mPlayerModel);
    mPlayerModel = NULL;
  }
}

void Player::Initialize()
{
  mPlayerModel = ModelUtils::CreateCubeModelPCNT();
  mSatelliteModel = ModelUtils::CreateCubeModelPCNT();
  mLightModel = ModelUtils::CreateCubeModelPCNT();

  XMFLOAT3 playerScale(5.0f, 5.0f, 5.0f);
  SetScale(playerScale);

  XMFLOAT3 satellitePosition(2.75f, 0.0f, 0.0f);
  mSatelliteModel->SetPosition(satellitePosition);

  XMFLOAT3 satelliteScale(0.5f, 0.5f, 0.5f);
  mSatelliteModel->SetScale(satelliteScale);
}

void Player::Update(float dt)
{
  XMFLOAT3 satelliteRotation = mSatelliteModel->GetRotation();
  satelliteRotation.y += 1.0f * dt;
  mSatelliteModel->SetRotation(satelliteRotation);

  static float scale = mSatelliteModel->GetScale().x;
  static float direction = 1.0f;
  scale += dt * direction;
  if (scale > 5.0f)
  {

  }

  mRotation.x -= 1.0f * dt;

  mPlayerModel->Update(dt);
  mSatelliteModel->Update(dt);
}

void Player::Paint(const XMMATRIX &world, Camera *camera, XMFLOAT3 lightPos)
{
  MatrixStack matrix;

  matrix.PushMatrix(XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z));
  matrix.PushMatrix(XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z));
  matrix.PushMatrix(XMMatrixScaling(mScale.x, mScale.y, mScale.z));

  mPlayerModel->Paint(matrix.GetCurrentMatrix(), camera, lightPos);

  XMFLOAT3 satellitePosition = mSatelliteModel->GetPosition();
  XMFLOAT3 satelliteRotation = mSatelliteModel->GetRotation();
  XMFLOAT3 satlliteScale = mSatelliteModel->GetScale();

  matrix.PushMatrix(XMMatrixScaling(satlliteScale.x, satlliteScale.y, satlliteScale.z)); // Scaling
  matrix.PushMatrix(XMMatrixRotationRollPitchYaw(satelliteRotation.x, satelliteRotation.y, satelliteRotation.z));
  matrix.PushMatrix(XMMatrixTranslation(satellitePosition.x, satellitePosition.y, satellitePosition.z));
  mSatelliteModel->Paint(matrix.GetCurrentMatrix(), camera, lightPos);
}