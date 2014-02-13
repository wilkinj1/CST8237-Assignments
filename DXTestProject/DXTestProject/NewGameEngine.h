#pragma once
#include "stdafx.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
class SceneManager;

class NewGameEngine
{
public:
  static NewGameEngine* GetInstance();
  static void DestroyInstance();

	virtual ~NewGameEngine();

	bool Initialize(HWND hWnd);
	void Update(float dt);
	void Render();

	SceneManager *GetSceneManager() const;

	ID3D11Device *GetD3DDevice();
	ID3D11DeviceContext *GetD3DDeviceContext();
	IDXGISwapChain *GetD3DSwapChain();
	ID3D11RenderTargetView *GetD3DBackBuffer();

protected:
  NewGameEngine();
  static NewGameEngine *sGameEngineInstance;

	ID3D11Device *mD3DDevice;
	ID3D11DeviceContext *mD3DDeviceContext;
	IDXGISwapChain *mD3DSwapChain;
	ID3D11RenderTargetView *mD3DBackBuffer;

	SceneManager *mSceneManager;
};