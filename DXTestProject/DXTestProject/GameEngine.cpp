#include "stdafx.h"
#include "GameEngine.h"
#include <D3D11.h>

GameEngine::GameEngine(void)
{
}


GameEngine::~GameEngine(void)
{
}


bool GameEngine::Initialize(HWND hWnd)
{
  /* Set up our DX11 environment. */
  RECT windowRect;
  GetClientRect(hWnd, &windowRect);
  bool successful = false;

  size_t windowWidth = windowRect.right - windowRect.left;
  size_t windowHeight = windowRect.bottom - windowRect.top;

  /* Setting up the description of our swap-chain so it will function as we desire. */
  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
  swapChainDesc.OutputWindow = GetActiveWindow();
  swapChainDesc.BufferCount = 1;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  // Running at 60 FPS (60MHz)
  swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

  // Extrapolate the width and height from the client rectangle.
  swapChainDesc.BufferDesc.Height = windowHeight;
  swapChainDesc.BufferDesc.Width = windowWidth;

  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;
  swapChainDesc.Windowed = true;

  /* Prioritize the driver types we want to use.
  * Hardware is fastest, WARP is an optimized software emulation, and Software is an unoptimized solution. */
  D3D_DRIVER_TYPE driverTypes[] = {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_SOFTWARE
  };

  /* Prioritize the feature level we want to use.
  * We're working with DX11 primarily, which works as a superset of DX10; any code written will work for both.
  * DX9 is different in its architecture, but it can be adapted. */
  D3D_FEATURE_LEVEL featureLevels[] = {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0
  };

  unsigned int creationFlags = 0;
#ifdef __DEBUG
  creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE selectedDriver;
  D3D_FEATURE_LEVEL selectedFeatureLevel;
  size_t numberOfDrivers = ARRAYSIZE(driverTypes);
  size_t numberOfFeatureLevels = ARRAYSIZE(featureLevels);

  HRESULT result = 0;

  for(size_t currentDriver = 0; currentDriver < numberOfDrivers; currentDriver++)
  {
    result = D3D11CreateDeviceAndSwapChain(NULL, 
      driverTypes[currentDriver], 
      0, 
      creationFlags, 
      featureLevels, 
      numberOfFeatureLevels, 
      D3D11_SDK_VERSION, 
      &swapChainDesc, 
      &mD3DSwapChain, 
      &mD3DDevice, 
      &selectedFeatureLevel, 
      &mD3DDeviceContext);

    if(SUCCEEDED(result))
    {
      selectedDriver = driverTypes[currentDriver];
      break;
    }
  }

  if(SUCCEEDED(result))
  {
    ID3D11Texture2D *backBufferTexture;
    if(SUCCEEDED(mD3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&backBufferTexture)))
    {
      if(SUCCEEDED(mD3DDevice->CreateRenderTargetView(backBufferTexture, 0, &mD3DBackBufferTarget)))
      {
        /* Since this is reference counted, we need to remove our reference since we're no longer using this object. */
        if(backBufferTexture != NULL)
        {
          backBufferTexture->Release();
        }

        mD3DDeviceContext->OMSetRenderTargets(1, &mD3DBackBufferTarget, NULL);
        CreateViewport(windowHeight, windowWidth);

        successful = true;
      }
      else
      {
        /* trace something out to let the user know that something went wrong with creating the render target view. */
      }
    }
    else 
    {
      /* trace something out to let the user know that something went wrong with getting the swap chain buffer. */
    }
  }
  else
  {
    /* trace something out to let the user know that something went wrong with creating the device. */
  }

  return successful;
}


void GameEngine::CreateViewport(size_t height, size_t width)
{
  D3D11_VIEWPORT mainViewport;
  mainViewport.Height = height;
  mainViewport.Width = width;
  mainViewport.MinDepth = 0.0f;
  mainViewport.MaxDepth = 1.0f;
  mainViewport.TopLeftX = 0.0f;
  mainViewport.TopLeftY = 0.0f;

  mD3DDeviceContext->RSSetViewports(1, &mainViewport);
}


bool GameEngine::CleanUp()
{
  if(mD3DBackBufferTarget)
  {
    mD3DBackBufferTarget->Release();
  }

  if(mD3DSwapChain)
  {
    mD3DSwapChain->Release();
  }

  if(mD3DDeviceContext)
  {
    mD3DDeviceContext->Release();
  }

  if(mD3DDevice)
  {
    mD3DDevice->Release();
  }

  return false;
}


void GameEngine::Update(float dt)
{
}


void GameEngine::Paint()
{
  static float clearColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
  mD3DDeviceContext->ClearRenderTargetView(mD3DBackBufferTarget, clearColor);

  mD3DSwapChain->Present(0, 0);
}