#include "stdafx.h"
#include "GameEngine.h"
#include <D3D11.h>

#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#include "SceneManager.h"
#include "GraphicsManager.h"
#include "GraphicsIncludes.h"
#include "AudioManager.h"

GameEngine* GameEngine::mEngineInstance = NULL;

GameEngine::GameEngine(void):
	mIsInitialized(false)
{
	mSceneManager = new SceneManager();
	mGraphicsManager = new GraphicsManager();
  mAudioManager = new AudioManager();
}

GameEngine::~GameEngine(void)
{
	if(mSceneManager != NULL)
	{
		delete mSceneManager;
	}

	if(mGraphicsManager != NULL)
	{
		delete mGraphicsManager;
	}

  if (mAudioManager)
  {
    delete mAudioManager;
  }
}

GameEngine* GameEngine::GetInstance()
{
	if(mEngineInstance == NULL)
	{
		mEngineInstance = new GameEngine();
	}

	return mEngineInstance;
}

void GameEngine::Shutdown()
{
	if(mEngineInstance != NULL)
	{
		delete mEngineInstance;
	}
}

bool GameEngine::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	if(!IsInitialized())
	{
		mIsInitialized = mGraphicsManager->Initialize(hWnd, hInstance);
    mAudioManager->Initialize();
    CreateConsoleWindow();
	}
	return IsInitialized();
}

bool GameEngine::IsInitialized()
{
	return mIsInitialized;
}

void GameEngine::Update(float dt)
{
	mSceneManager->Update(dt);
}

void GameEngine::Paint()
{
	static float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mGraphicsManager->mD3DDeviceContext->ClearRenderTargetView(mGraphicsManager->mD3DBackBufferTarget, clearColor);
#ifndef NO_DEPTH
  mGraphicsManager->mD3DDeviceContext->ClearDepthStencilView(mGraphicsManager->mD3DDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
#endif

	mSceneManager->Paint();

	mGraphicsManager->mD3DSwapChain->Present(0, 0);
}

GraphicsManager* GameEngine::GetGraphicsManager()
{
	return mGraphicsManager;
}

SceneManager* GameEngine::GetSceneManager()
{
	return mSceneManager;
}

void GameEngine::CreateConsoleWindow()
{
  AllocConsole();

  int hConHandle;
  long lStdHandle;
  CONSOLE_SCREEN_BUFFER_INFO conInfo;
  FILE *fp;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);
  conInfo.dwSize.Y = 1000;
  SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), conInfo.dwSize);

  // output
  lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

  fp = _fdopen(hConHandle, "w");
  *stdout = *fp;

  setvbuf(stdout, NULL, _IONBF, 0);

  // input
  lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

  fp = _fdopen(hConHandle, "r");
  *stdout = *fp;

  setvbuf(stdout, NULL, _IONBF, 0);

  // error
  lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

  fp = _fdopen(hConHandle, "w");
  *stdout = *fp;

  setvbuf(stdout, NULL, _IONBF, 0);

  std::ios::sync_with_stdio();
}
