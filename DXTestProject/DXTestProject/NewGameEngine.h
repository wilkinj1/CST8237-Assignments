#pragma once
#include "stdafx.h"

class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;

class NewGameEngine
{
public:
	NewGameEngine();
	virtual ~NewGameEngine();

	bool Initialize(HWND hWnd);
	void Update(float dt);
	void Render();

protected:
	ID3D11Device *mD3DDevice;
	ID3D11DeviceContext *mD3DDeviceContext;
	IDXGISwapChain *mD3DSwapChain;
	ID3D11RenderTargetView *mD3DBackBuffer;
};