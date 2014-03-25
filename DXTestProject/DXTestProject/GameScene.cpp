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
  mSatellitePlayer = NULL;
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

  if (mSatellitePlayer)
  {
    delete mSatellitePlayer;
    mSatellitePlayer = NULL;
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

  mSatellitePlayer = new Player();
  mSatellitePlayer->Initialize();

  mLightModel = ModelUtils::CreateCubeModelPCNT();

  mLight.position = XMFLOAT3(0.0f, 15.0f, 0.0f);
  mLight.ambientColor = XMFLOAT3(0.2f, 0.2f, 0.2f);
  mLight.diffuseColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
  mLight.intensity = 1.0f;

  XMFLOAT3 playerScale(5.0f, 5.0f, 5.0f);
  mPlayer->SetScale(playerScale);

  XMFLOAT3 satellitePosition(2.5f, 0.0f, 0.0f);
  mSatellitePlayer->SetPosition(satellitePosition);

  XMFLOAT3 satelliteScale(0.5f, 0.5f, 0.5f);
  mSatellitePlayer->SetScale(satelliteScale);

  CollisionMesh playerMesh;
  playerMesh.Create(mPlayer->GetModel(), mPlayer);
  mCollidableObjects.push_back(playerMesh);

  CollisionMesh satelliteMesh;
  satelliteMesh.Create(mSatellitePlayer->GetModel(), mSatellitePlayer);
  mCollidableObjects.push_back(satelliteMesh);

	mIsInitialized = true;
}

void GameScene::Update(float dt)
{
  XMFLOAT3 satelliteRotation = mSatellitePlayer->GetRotation();
  satelliteRotation.y += 1.0f * dt;
  mSatellitePlayer->SetRotation(satelliteRotation);

  XMFLOAT3 playerRotation = mPlayer->GetRotation();
  playerRotation.x -= 1.0f * dt;
  mPlayer->SetRotation(playerRotation);

	mPlayer->Update(dt);
  mSatellitePlayer->Update(dt);

	mSceneCamera->Update(dt);

  mLightModel->Update(dt);

  CheckCollisions();
}

void GameScene::Paint()
{
  MatrixStack matrix;
  matrix.PushMatrix(XMMatrixScaling(1.0f, 1.0f, 1.0f));
  matrix.PushMatrix(XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f));
  matrix.PushMatrix(XMMatrixTranslation(mLight.position.x, mLight.position.y, mLight.position.z));
  mLightModel->Paint(matrix.GetCurrentMatrix(), mSceneCamera, mLight.position);

  mPlayer->Paint(XMMatrixIdentity(), mSceneCamera, mLight.position);

  /* We now have to position our new satellite player based on where our player is. This is as simple as calculating our
   * player's world matrix, and then just passing it into the Player; it handles the rest itself!*/
  matrix.Clear();
  matrix.PushMatrix(XMMatrixTranslation(mPlayer->GetPosition().x, mPlayer->GetPosition().y, mPlayer->GetPosition().z));
  matrix.PushMatrix(XMMatrixRotationRollPitchYaw(mPlayer->GetRotation().x, mPlayer->GetRotation().y, mPlayer->GetRotation().z));
  matrix.PushMatrix(XMMatrixScaling(mPlayer->GetScale().x, mPlayer->GetScale().y, mPlayer->GetScale().z));

  mSatellitePlayer->Paint(matrix.GetCurrentMatrix(), mSceneCamera, mLight.position);
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

void GameScene::CheckCollisions()
{
  for (int firstCollidableIndex = 0; firstCollidableIndex + 1 < mCollidableObjects.size(); firstCollidableIndex++)
  {
    CollisionMesh &firstMesh = mCollidableObjects[firstCollidableIndex];
    for (int secondCollidableIndex = firstCollidableIndex + 1; secondCollidableIndex < mCollidableObjects.size(); secondCollidableIndex++)
    {
      CollisionMesh &secondMesh = mCollidableObjects[secondCollidableIndex];
      if (firstMesh.CheckCollisions(secondMesh))
      {
        printf("We have a collision!");
      }
    }
  }
}