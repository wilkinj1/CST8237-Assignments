#include "stdafx.h"
#include "GameScene.h"
#include <d3d11.h>
#include "Player.h"
#include "Camera.h"

#include "GameEngine.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "MatrixStack.h"
#include "Model.h"
#include "BasicShaderModelBuilder.h"
#include "ModelUtils.h"

GameScene::GameScene(): Scene()
{
	mPlayer = NULL;
  mDT = 10.0f;
  mScore = 0;
}

GameScene::~GameScene()
{
	if(mPlayer)
	{
		delete mPlayer;
    mPlayer = NULL;
	}

  if (mLightModel)
  {
    BasicShaderModelBuilder::Destroy(mLightModel);
    mLightModel = NULL;
  }
}

void GameScene::Initialize()
{
	mSceneCamera = new Camera(XM_PIDIV4, 1424.0f, 702.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, -50.0f, 1.0f));
	mPlayer = new Player();
	mPlayer->Initialize();

  mLightModel = ModelUtils::CreateCubeModelPCNT();

  mLight.position = XMFLOAT3(0.0f, 15.0f, 0.0f);
  mLight.ambientColor = XMFLOAT3(0.2f, 0.2f, 0.2f);
  mLight.diffuseColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
  mLight.intensity = 1.0f;

	mIsInitialized = true;
}

void GameScene::Update(float dt)
{
	mPlayer->Update(dt);
	mSceneCamera->Update(dt);

  mLightModel->Update(dt);
}

void GameScene::Paint()
{
  MatrixStack matrix;
  matrix.PushMatrix(XMMatrixScaling(1.0f, 1.0f, 1.0f));
  matrix.PushMatrix(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f));
  matrix.PushMatrix(XMMatrixTranslation(mLight.position.x, mLight.position.y, mLight.position.z));
  mLightModel->Paint(matrix.GetCurrentMatrix(), mSceneCamera, mLight.position);

  mPlayer->Paint(XMMatrixIdentity(), mSceneCamera, mLight.position);
}

void GameScene::OnEnter() {   }
void GameScene::OnExit()  
{
}

void GameScene::HandleInput(UINT wParam, UINT lParam)
{
  switch(wParam)
  {
  case('A'):
    {
      XMFLOAT4 position = mSceneCamera->GetPosition();
      position.x -= 1.0f;
      mSceneCamera->SetPosition(position);
    }
    break;
  case('W'):
    {
      XMFLOAT4 position = mSceneCamera->GetPosition();
      position.z += 1.0f;
      mSceneCamera->SetPosition(position);
    }
    break;
  case('S'):
    {
      XMFLOAT4 position = mSceneCamera->GetPosition();
      position.z -= 1.0f;
      mSceneCamera->SetPosition(position);
    }
    break;
  case('D'):
    {
      XMFLOAT4 position = mSceneCamera->GetPosition();
      position.x += 1.0f;
      mSceneCamera->SetPosition(position);
    }
    break;
  case('Q'):
    {
      XMFLOAT4 position = mSceneCamera->GetPosition();
      position.y += 1.0f;
      mSceneCamera->SetPosition(position);
    }
    break;
  case('E'):
    {
      XMFLOAT4 position = mSceneCamera->GetPosition();
      position.y -= 1.0f;
      mSceneCamera->SetPosition(position);
    }
    break;
  }
}
