#include "stdafx.h"
#include "GameScreen.h"
#include <d3d11.h>
#include "Player.h"
#include "Camera.h"

#include "NewGameEngine.h"
#include "ScreenManager.h"

GameScreen::GameScreen() : Screen()
{
  mPlayer = NULL;
  mDT = 10.0f;
  mScore = 0;
}

GameScreen::~GameScreen()
{
  if (mPlayer)
  {
    delete mPlayer;
    mPlayer = NULL;
  }
}

void GameScreen::Initialize()
{
  mSceneCamera = new Camera(XM_PIDIV4, 1424.0f / 702.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, -5.0f, 1.0f));
  mPlayer = new Player();
  mPlayer->Initialize();
  mIsInitialized = true;
}

void GameScreen::Update(float dt)
{
  mPlayer->Update(dt);
  mSceneCamera->Update(dt);
}

void GameScreen::Render()
{
  mPlayer->Render(mSceneCamera->GetViewProjectionMatrix());
}

void GameScreen::OnEnter() {   }
void GameScreen::OnExit()
{
}

void GameScreen::HandleInput(UINT wParam, UINT lParam)
{
  switch (wParam)
  {
  case('A') :
  {
              XMFLOAT4 position = mSceneCamera->GetPosition();
              position.x -= 0.1f;
              mSceneCamera->SetPosition(position);
  }
            break;
  case('W') :
  {
              XMFLOAT4 position = mSceneCamera->GetPosition();
              position.z += 0.1f;
              mSceneCamera->SetPosition(position);
  }
            break;
  case('S') :
  {
              XMFLOAT4 position = mSceneCamera->GetPosition();
              position.z -= 0.1f;
              mSceneCamera->SetPosition(position);
  }
            break;
  case('D') :
  {
              XMFLOAT4 position = mSceneCamera->GetPosition();
              position.x += 0.1f;
              mSceneCamera->SetPosition(position);
  }
            break;
  case('Q') :
  {
              XMFLOAT4 position = mSceneCamera->GetPosition();
              position.y += 0.1f;
              mSceneCamera->SetPosition(position);
  }
            break;
  case('E') :
  {
              XMFLOAT4 position = mSceneCamera->GetPosition();
              position.y -= 0.1f;
              mSceneCamera->SetPosition(position);
  }
            break;
  }
}