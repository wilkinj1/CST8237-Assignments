#include "stdafx.h"
#include "NewGameEngine.h"
#include <d3d11.h>
#include "ScreenManager.h"

NewGameEngine* NewGameEngine::sGameEngineInstance = NULL;

NewGameEngine* NewGameEngine::GetInstance()
{
  if(!sGameEngineInstance)
  {
    sGameEngineInstance = new NewGameEngine();
  }

  return sGameEngineInstance;
}

void NewGameEngine::DestroyInstance()
{
  if(sGameEngineInstance)
  {
    delete sGameEngineInstance;
    sGameEngineInstance = NULL;
  }
}

NewGameEngine::NewGameEngine():
	mD3DDevice(NULL),
	mD3DDeviceContext(NULL),
	mD3DSwapChain(NULL),
	mD3DBackBuffer(NULL),
	mScreenManager(NULL)
{
}

NewGameEngine::~NewGameEngine()
{
  if (mD3DDepthBufferTexture)
  {
    mD3DDepthBufferTexture->Release();
    mD3DDepthBufferTexture = NULL;
  }

  if (mD3DDepthStencilView)
  {
    mD3DDepthStencilView->Release();
    mD3DDepthStencilView = NULL;
  }

  if (mD3DSwapChain)
  {
    mD3DSwapChain->Release();
    mD3DSwapChain = NULL;
  }

  if (mD3DDeviceContext)
  {
    mD3DDeviceContext->Release();
    mD3DDeviceContext = NULL;
  }

  if (mD3DDevice)
  {
    mD3DDevice->Release();
    mD3DDevice = NULL;
  }
}

bool NewGameEngine::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	bool successful = false;

  this->hWnd = hWnd;
  this->hInstance = hInstance;

	// Creating our swap-chain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	int screenWidth = clientRect.right - clientRect.left;
	int screenHeight = clientRect.bottom - clientRect.top;

	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	// Prioritize driver types for when our device is created.
	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE
	};

	size_t numberOfDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	size_t numberOfFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_DRIVER_TYPE selectedDriver;
	D3D_FEATURE_LEVEL selectedLevel;

	HRESULT result;

	for(size_t currentDriver = 0; currentDriver < numberOfDriverTypes; currentDriver++)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL,
			driverTypes[currentDriver],
			0,
#if __DEBUG
			D3D11_CREATE_DEVICE_DEBUG
#else
			0
#endif
			, featureLevels,
			numberOfFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&mD3DSwapChain,
			&mD3DDevice,
			&selectedLevel,
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
		result = mD3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&backBufferTexture);
		if(SUCCEEDED(result))
		{
			result = mD3DDevice->CreateRenderTargetView(backBufferTexture, 0, &mD3DBackBuffer);
			if(SUCCEEDED(result))
			{
				/* Since this is reference counted, we need to remove our reference since we're no longer using this object. */
				if(backBufferTexture != NULL)
				{
					backBufferTexture->Release();
				}

        D3D11_TEXTURE2D_DESC depthDesc;
        ZeroMemory(&depthDesc, sizeof(depthDesc));
        depthDesc.ArraySize = 1;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.Width = screenWidth;
        depthDesc.Height = screenHeight;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.MipLevels = 1;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.SampleDesc.Quality = 0;

        result = mD3DDevice->CreateTexture2D(&depthDesc, NULL, &mD3DDepthBufferTexture);

        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
        depthStencilViewDesc.Format = depthDesc.Format;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;

        result = mD3DDevice->CreateDepthStencilView(mD3DDepthBufferTexture, &depthStencilViewDesc, &mD3DDepthStencilView);

				// Tell the device context that we want to render to the render target.
        mD3DDeviceContext->OMSetRenderTargets(1, &mD3DBackBuffer, mD3DDepthStencilView);

				// Creating our viewport.
				D3D11_VIEWPORT mainViewport;
				mainViewport.Height = screenHeight;
				mainViewport.Width = screenWidth;
				mainViewport.MinDepth = 0.0f;
				mainViewport.MaxDepth = 1.0f;
				mainViewport.TopLeftX = 0.0f;
				mainViewport.TopLeftY = 0.0f;

				// Setting our viewport.
				mD3DDeviceContext->RSSetViewports(1, &mainViewport);

				// Initialize our scene manager now that we know everything's been properly set up.
        mScreenManager = new ScreenManager();

				// Now that we know that we've properly created our viewport, we can report a success.
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

const HWND& NewGameEngine::GetHWND()
{
  return hWnd;
}
  
const HINSTANCE& NewGameEngine::GetHINSTANCE()
{
  return hInstance;
}

ScreenManager* NewGameEngine::GetScreenManager() const
{
  return mScreenManager;
}

void NewGameEngine::Update(float dt)
{
  mScreenManager->Update(dt);
}

void NewGameEngine::Render()
{
  static float clearColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };
  mD3DDeviceContext->ClearRenderTargetView(mD3DBackBuffer, clearColor);
  mD3DDeviceContext->ClearDepthStencilView(mD3DDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

  // DRAWING STUFF GOES HERE.
  mScreenManager->Render();

  mD3DSwapChain->Present(0, 0);
}

ID3D11Device* NewGameEngine::GetD3DDevice()
{
  return mD3DDevice;
}

ID3D11DeviceContext* NewGameEngine::GetD3DDeviceContext()
{
  return mD3DDeviceContext;
}

IDXGISwapChain* NewGameEngine::GetD3DSwapChain()
{
  return mD3DSwapChain;
}

ID3D11RenderTargetView* NewGameEngine::GetD3DBackBuffer()
{
  return mD3DBackBuffer;
}