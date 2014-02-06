#pragma once
#include "windows.h"

class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;

class GameEngine
{
public:
  GameEngine(void);
  virtual ~GameEngine(void);

  bool Initialize(HWND hWnd);
  bool CleanUp();

  void CreateViewport(size_t height, size_t width);

  void Update(float dt);
  void Paint();

private:
  ID3D11Device *mD3DDevice;
  ID3D11DeviceContext *mD3DDeviceContext;
  IDXGISwapChain *mD3DSwapChain;

  ID3D11RenderTargetView *mD3DBackBufferTarget;
};

