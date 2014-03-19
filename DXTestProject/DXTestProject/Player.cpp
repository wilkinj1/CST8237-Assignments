#include "stdafx.h"
#include "Player.h"
#include <assert.h>
#include <string>

#include "GameEngine.h"
#include "GraphicsManager.h"

#include <d3dcommon.h>
#include "Model.h"
#include "ModelBuilder.h"
#include "ModelUtils.h"

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
    ModelBuilder::destroy(mPlayerModel);
    mPlayerModel = NULL;
  }
}

void Player::Initialize()
{
  VertexPositionColorNormalTextureUVDescription verticesCombo[] =
  {
    // front face
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },

    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

    // right side face
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

    { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

    // back face
    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

    { XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

    // left side face
    { XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

    { XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

    // top face
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

    { XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    
    // bottom face
    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

    { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },

    //?
    { XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }
    //{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
    //{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
  };

  D3D11_INPUT_ELEMENT_DESC elementDesciptions[] = 
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  MODEL_DESC modelDescription;
  modelDescription.pData = verticesCombo;
  modelDescription.sizeOfData = sizeof(verticesCombo);
  modelDescription.sizeOfVertexDesc = sizeof(VertexPositionColorNormalTextureUVDescription);
  modelDescription.vertexCount = ARRAYSIZE(verticesCombo);
  modelDescription.elementDescs = elementDesciptions;
  modelDescription.elementCount = ARRAYSIZE(elementDesciptions);

  mPlayerModel = ModelBuilder::create(modelDescription);
  mSatelliteModel = ModelBuilder::create(modelDescription);
  mLightModel = ModelBuilder::create(modelDescription);

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

void Player::Paint(const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &proj, const XMFLOAT4 &cameraPos, XMFLOAT4 lightPos)
{
  XMMATRIX viewProj = view * proj;

  XMMATRIX playerTranslation = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
  XMMATRIX playerRotation = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
  XMMATRIX playerScale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);

  XMMATRIX playerWorldMatrix = playerScale * playerRotation * playerTranslation;
  mPlayerModel->Paint(playerWorldMatrix, view, proj, cameraPos, lightPos);

  XMFLOAT3 satellitePosition = mSatelliteModel->GetPosition();
  XMFLOAT3 satelliteRotation = mSatelliteModel->GetRotation();
  XMFLOAT3 satlliteScale = mSatelliteModel->GetScale();

  XMMATRIX satelliteTranslationMatrix = XMMatrixTranslation(satellitePosition.x, satellitePosition.y, satellitePosition.z);
  XMMATRIX satelliteRotationMatrix = XMMatrixRotationRollPitchYaw(satelliteRotation.x, satelliteRotation.y, satelliteRotation.z);
  XMMATRIX satelliteScaleMatrix = XMMatrixScaling(satlliteScale.x, satlliteScale.y, satlliteScale.z);

  XMMATRIX satelliteWorldMatrix = satelliteTranslationMatrix * satelliteRotationMatrix * satelliteScaleMatrix * playerWorldMatrix;
  mSatelliteModel->Paint(satelliteWorldMatrix, view, proj, cameraPos, lightPos);

  XMMATRIX lightTranslationMatrix = XMMatrixTranslation(lightPos.x, lightPos.y, lightPos.z);
  XMMATRIX lightRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
  XMMATRIX lightScaleMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);

  XMMATRIX lightWorldMatrix = lightTranslationMatrix * lightRotationMatrix * lightScaleMatrix;
  mLightModel->Paint(lightWorldMatrix, view, proj, cameraPos, lightPos);
}