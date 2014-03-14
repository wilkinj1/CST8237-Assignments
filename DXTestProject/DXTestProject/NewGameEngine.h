#pragma once
#include "stdafx.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
class ScreenManager;

class NewGameEngine
{
public:
  static NewGameEngine* GetInstance();
  static void DestroyInstance();

	virtual ~NewGameEngine();

	bool Initialize(HWND hWnd, HINSTANCE hInstance);
	void Update(float dt);
	void Render();

  const HWND& GetHWND();
  const HINSTANCE& GetHINSTANCE();

  ScreenManager *GetScreenManager() const;

	ID3D11Device *GetD3DDevice();
	ID3D11DeviceContext *GetD3DDeviceContext();
	IDXGISwapChain *GetD3DSwapChain();
	ID3D11RenderTargetView *GetD3DBackBuffer();

protected:
  NewGameEngine();
  static NewGameEngine *sGameEngineInstance;

  HWND hWnd;
  HINSTANCE hInstance;

	ID3D11Device *mD3DDevice;
	ID3D11DeviceContext *mD3DDeviceContext;
	IDXGISwapChain *mD3DSwapChain;
	ID3D11RenderTargetView *mD3DBackBuffer;

  ID3D11Texture2D *mD3DDepthBufferTexture;
  ID3D11DepthStencilView *mD3DDepthStencilView;

  ScreenManager *mScreenManager;
};