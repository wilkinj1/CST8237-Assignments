#include "stdafx.h"
#include "GameEngine.h"
//#include <d3d.h>
#include <D3D11.h>

#include "SceneManager.h"
#include "GraphicsManager.h"
#include "Paintable.h"

GameEngine* GameEngine::mEngineInstance = NULL;

GameEngine::GameEngine(void):
	mIsInitialized(false)
{
	mSceneManager = new SceneManager();
	mGraphicsManager = new GraphicsManager();
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
}

GameEngine* GameEngine::getInstance()
{
	if(mEngineInstance == NULL)
	{
		mEngineInstance = new GameEngine();
	}

	return mEngineInstance;
}

void GameEngine::shutdown()
{
	if(mEngineInstance != NULL)
	{
		delete mEngineInstance;
	}
}

bool GameEngine::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	if(!isInitialized())
	{
		mIsInitialized = mGraphicsManager->Initialize(hWnd, hInstance);
	}
	return isInitialized();
}

bool GameEngine::isInitialized()
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

GraphicsManager* GameEngine::getGraphicsManager()
{
	return mGraphicsManager;
}

SceneManager* GameEngine::getSceneManager()
{
	return mSceneManager;
}