#include "stdafx.h"
#include "GameScene.h"
#include <d3d11.h>
#include "Player.h"
#include "Obstacle.h"
#include "FocusCamera.h"

#include "GameEngine.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "MatrixStack.h"
#include "Model.h"
#include "BasicShaderModelBuilder.h"
#include "ModelUtils.h"

#include "EasyFont.h"
#include "Animation.h"

GameScene::GameScene(): Scene()
{
	mObstacle = NULL;
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

  if (mObstacle)
  {
    delete mObstacle;
    mObstacle = NULL;
  }

  if (mLightModel)
  {
    BasicShaderModelBuilder::Destroy(mLightModel);
    mLightModel = NULL;
  }
}

void GameScene::Initialize()
{
  GraphicsManager *gm = GameEngine::GetInstance()->GetGraphicsManager();

  RECT windowRect;
  GetClientRect(gm->GetWindowHandle(), &windowRect);

	size_t windowWidth = windowRect.right - windowRect.left;
	size_t windowHeight = windowRect.bottom - windowRect.top;
	mSceneCamera = new FocusCamera(XM_PIDIV4, windowWidth, windowHeight, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(5.0f, 0.0f, -20.0f, 1.0f));
	
  mObstacle = new Obstacle();
	mObstacle->Initialize();

  mPlayer = new Player();
  mPlayer->Initialize();

  mFont = new EasyFont();
  mFont->Initialize(gm->GetGraphicsDevice(), gm->GetGraphicsDeviceContext());

  Animation anim;
  anim.Initialize("./trogdor.anim");

  mLightModel = ModelUtils::CreateCubeModelPCNT();

  mLight.position = XMFLOAT3(0.0f, 15.0f, 0.0f);
  mLight.ambientColor = XMFLOAT3(0.2f, 0.2f, 0.2f);
  mLight.diffuseColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
  mLight.intensity = 1.0f;

  XMFLOAT3 obstacleScale(5.0f, 5.0f, 5.0f);
  mObstacle->SetScale(obstacleScale);

  XMFLOAT3 obstaclePosition(0.0f, 0.0f, 0.0f);
  mObstacle->SetPosition(obstaclePosition);

  XMFLOAT3 playerPosition(8.0f, 0.0f, 0.0f);
  mPlayer->SetPosition(playerPosition);

  mSceneCamera->SetLookAtTarget(XMFLOAT4(obstaclePosition.x, obstaclePosition.y, obstaclePosition.z, 1.0f));

  CollisionMesh playerMesh;
  playerMesh.Create(mPlayer->GetModel(), mPlayer);
  mCollidableObjects.push_back(playerMesh);

  CollisionMesh obstacleMesh;
  obstacleMesh.Create(mObstacle->GetModel(), mObstacle);
  mCollidableObjects.push_back(obstacleMesh);

	mIsInitialized = true;
}

void GameScene::ViewSizeChanged()
{
  RECT windowRect;
  GetClientRect(GameEngine::GetInstance()->GetGraphicsManager()->GetWindowHandle(), &windowRect);

	size_t windowWidth = windowRect.right - windowRect.left;
	size_t windowHeight = windowRect.bottom - windowRect.top;
  mSceneCamera->SetViewSize(windowWidth, windowHeight);
}

void GameScene::Update(float dt)
{
  HandleInput(dt);

  mPlayer->Update(dt);
  mObstacle->Update(dt);

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
  //matrix.Clear();
  //matrix.PushMatrix(XMMatrixTranslation(mPlayer->GetPosition().x, mPlayer->GetPosition().y, mPlayer->GetPosition().z));
  //matrix.PushMatrix(XMMatrixRotationRollPitchYaw(mPlayer->GetRotation().x, mPlayer->GetRotation().y, mPlayer->GetRotation().z));
  //matrix.PushMatrix(XMMatrixScaling(mPlayer->GetScale().x, mPlayer->GetScale().y, mPlayer->GetScale().z));

  mObstacle->Paint(XMMatrixIdentity(), mSceneCamera, mLight.position);

  ID3D11DeviceContext *dc = GameEngine::GetInstance()->GetGraphicsManager()->GetGraphicsDeviceContext();
  mFont->DrawString(dc, mSceneCamera, "HELLO!", 0, 0);
}

void GameScene::OnEnter() {   }
void GameScene::OnExit()  
{
}

void GameScene::HandleInput(float dt)
{
  Player *movingPlayer = mPlayer;
  const float PlayerMovementSpeed = 5.0f;
  const float CameraMovementSpeed = 5.0f;
  const float CameraRotation = 20.0f;

  // Cache the old player position so that we can set the position back to it.
  XMFLOAT3 oldPlayerPosition = movingPlayer->GetPosition();

  if (GetAsyncKeyState('A'))
  {
    XMFLOAT4 position = mSceneCamera->GetPosition();
    position.x -= CameraMovementSpeed * dt;
    //mSceneCamera->SetPosition(position);

    XMFLOAT3 angle(0.0f, XMConvertToRadians(CameraRotation) * dt, 0.0f);
    ((FocusCamera *)mSceneCamera)->RotateBy(angle);
  }
  else if (GetAsyncKeyState('D'))
  {
    XMFLOAT3 angle(0.0f, XMConvertToRadians(-CameraRotation) * dt, 0.0f);
    ((FocusCamera *)mSceneCamera)->RotateBy(angle);
  }
  
  if (GetAsyncKeyState('W'))
  {
    XMFLOAT4 position = mSceneCamera->GetPosition();
    position.z += CameraMovementSpeed * dt;
    mSceneCamera->SetPosition(position);
  }
  else if (GetAsyncKeyState('S'))
  {
    XMFLOAT4 position = mSceneCamera->GetPosition();
    position.z -= CameraMovementSpeed * dt;
    mSceneCamera->SetPosition(position);
  }

  if (GetAsyncKeyState('Q'))
  {
    XMFLOAT4 position = mSceneCamera->GetPosition();
    position.y += CameraMovementSpeed * dt;
    mSceneCamera->SetPosition(position);
  }
  else if (GetAsyncKeyState('E'))
  {
    XMFLOAT4 position = mSceneCamera->GetPosition();
    position.y -= CameraMovementSpeed * dt;
    mSceneCamera->SetPosition(position);
  }

  if (GetAsyncKeyState('J'))
  {
    XMFLOAT3 position = oldPlayerPosition;
    position.x -= PlayerMovementSpeed * dt;
    movingPlayer->SetPosition(position);
  }
  else if (GetAsyncKeyState('L'))
  {
    XMFLOAT3 position = oldPlayerPosition;
    position.x += PlayerMovementSpeed * dt;
    movingPlayer->SetPosition(position);
  }

  if (GetAsyncKeyState('I'))
  {
    XMFLOAT3 position = oldPlayerPosition;
    position.y += PlayerMovementSpeed * dt;
    movingPlayer->SetPosition(position);
  }
  else if (GetAsyncKeyState('K'))
  {
    XMFLOAT3 position = oldPlayerPosition;
    position.y -= PlayerMovementSpeed * dt;
    movingPlayer->SetPosition(position);
  }

  if (GetAsyncKeyState('X'))
  {
    PostQuitMessage(0);
  }

  // if we're colliding, we set the position back to what it was before.
  //movingPlayer->SetPosition(oldPlayerPosition);
}

bool GameScene::CheckCollisions()
{
  bool collisionOccurred = false;
  for (int firstCollidableIndex = 0; firstCollidableIndex + 1 < mCollidableObjects.size(); firstCollidableIndex++)
  {
    CollisionMesh &firstMesh = mCollidableObjects[firstCollidableIndex];
    for (int secondCollidableIndex = firstCollidableIndex + 1; secondCollidableIndex < mCollidableObjects.size(); secondCollidableIndex++)
    {
      CollisionMesh &secondMesh = mCollidableObjects[secondCollidableIndex];

      if (&firstMesh != &secondMesh)
      {
        collisionOccurred = firstMesh.CheckCollisionsCustom(secondMesh);
      }
    }
  }

  return collisionOccurred;
}