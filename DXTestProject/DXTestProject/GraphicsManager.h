#pragma once
#include <windows.h>
#include <string>
#include <D3Dcommon.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

class GameEngine;

class GraphicsManager
{
	friend class GameEngine;

public:
	virtual ~GraphicsManager(void);

  void SetViewport(float height, float width);

	ID3D11Device *GetGraphicsDevice();
	ID3D11DeviceContext *GetGraphicsDeviceContext();

  HWND GetWindowHandle();
  HINSTANCE GetWindowInstance();

	static HRESULT CompileShaderFromFile(const wchar_t *filename, const char *shaderEntryPoint, const char *shaderTarget, LPD3DBLOB *shaderData, LPD3DBLOB *errorData);

protected:
	GraphicsManager(void);
	bool Initialize(HWND hWnd, HINSTANCE hInstance);

  HWND mHWnd;
  HINSTANCE mHInstance;

	ID3D11Device *mD3DDevice;
	ID3D11DeviceContext *mD3DDeviceContext;
	IDXGISwapChain *mD3DSwapChain;

	ID3D11RenderTargetView *mD3DBackBufferTarget;

  ID3D11Texture2D *mD3DDepthBufferTexture;
  ID3D11DepthStencilView *mD3DDepthStencilView;
};
