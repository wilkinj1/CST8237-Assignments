#include "stdafx.h"
#include "GraphicsManager.h"
#include <D3D11.h>
#include <d3dcompiler.h>
#include <stdio.h>
#include "GraphicsIncludes.h"

#ifndef OLD_DX_SDK
#include "WICTextureLoader.cpp"
#include "DirectXTex/DirectXTex.h"
#pragma comment(lib, "DirectXTex.lib")
#endif

GraphicsManager::GraphicsManager() :
  mD3DBackBufferTarget(NULL),
  mD3DSwapChain(NULL),
  mD3DDeviceContext(NULL),
  mD3DDevice(NULL),
  mD3DDepthBufferTexture(NULL),
  mD3DDepthStencilView(NULL)
{
}

GraphicsManager::~GraphicsManager()
{
  if(mD3DDepthBufferTexture)
  {
    mD3DDepthBufferTexture->Release();
    mD3DDepthBufferTexture = NULL;
  }

  if(mD3DDepthStencilView)
  {
    mD3DDepthStencilView->Release();
    mD3DDepthStencilView = NULL;
  }

	if(mD3DBackBufferTarget)
	{
		mD3DBackBufferTarget->Release();
    mD3DBackBufferTarget = NULL;
	}

	if(mD3DSwapChain)
	{
		mD3DSwapChain->Release();
    mD3DSwapChain = NULL;
	}

	if(mD3DDeviceContext)
	{
		mD3DDeviceContext->Release();
    mD3DDeviceContext = NULL;
	}

	if(mD3DDevice)
	{
		mD3DDevice->Release();
    mD3DDevice = NULL;
	}
}

bool GraphicsManager::Initialize(HWND hWnd, HINSTANCE hInstance)
{
  mHWnd = hWnd;
  mHInstance = hInstance;

	/* Set up our DX11 environment. */
	RECT windowRect;
	GetClientRect(hWnd, &windowRect);
	bool successful = false;

	size_t windowWidth = windowRect.right - windowRect.left;
	size_t windowHeight = windowRect.bottom - windowRect.top;

	/* Setting up the description of our swap-chain so it will function as we desire. */
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.OutputWindow = hWnd;
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

        D3D11_TEXTURE2D_DESC depthDesc;
        ZeroMemory(&depthDesc, sizeof(depthDesc));
        depthDesc.ArraySize = 1;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.Width = windowWidth;
        depthDesc.Height = windowHeight;
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

        mD3DDeviceContext->OMSetRenderTargets(1, &mD3DBackBufferTarget, mD3DDepthStencilView);
				SetViewport((float)windowHeight, (float)windowWidth);

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

HWND GraphicsManager::GetWindowHandle()
{
  return mHWnd;
}

HINSTANCE GraphicsManager::GetWindowInstance()
{
  return mHInstance;
}

void GraphicsManager::SetViewport(float height, float width)
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

ID3D11Device* GraphicsManager::GetGraphicsDevice()
{
	return mD3DDevice;
}

ID3D11DeviceContext* GraphicsManager::GetGraphicsDeviceContext()
{
	return mD3DDeviceContext;
}

HRESULT GraphicsManager::CompileShaderFromFile(const wchar_t *filename, const char *shaderEntryPoint, const char *shaderTarget, LPD3DBLOB *shaderData, LPD3DBLOB *errorData)
{
  HRESULT result = E_FAIL;

#ifdef OLD_DX_SDK
  result = D3DX11CompileFromFile(filename, NULL, NULL, shaderEntryPoint, shaderTarget, 0, 0, NULL, shaderData, errorData, NULL);
#else
	HANDLE fileHandle = CreateFile(filename, 
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD fileSize = GetFileSize(fileHandle, NULL);
	char *fileBuffer = new char[fileSize];
	DWORD bytesRead = 0;

	if(ReadFile(fileHandle, fileBuffer, fileSize, &bytesRead, NULL))
	{
		result = D3DCompile(fileBuffer, 
			fileSize, 
			shaderEntryPoint, 
			NULL, 
			NULL, 
			shaderEntryPoint,
			shaderTarget,
			0,
			0,
			shaderData,
			errorData);
	}

	delete[] fileBuffer;
#endif
	return result;
}

HRESULT GraphicsManager::CreateShaderResourceViewFromFile(ID3D11Device *device, ID3D11DeviceContext *dc, const wchar_t *filename, ID3D11ShaderResourceView **resourceView)
{
  HRESULT result = E_FAIL;

#ifdef OLD_DX_SDK
	//If not, then we have to load it!
	D3DX11_IMAGE_LOAD_INFO imageInfo;
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, &imageInfo, NULL, (ID3D11ShaderResourceView **)&resourceView, NULL);
#else
  ID3D11Texture2D *tex;
  result = CreateWICTextureFromFile(device, dc, filename, (ID3D11Resource **)&tex, resourceView);
  if (FAILED(result))
  {
    DirectX::TexMetadata md;
    DirectX::ScratchImage img;
    result = LoadFromDDSFile(filename, 0, &md, img);
    result = CreateShaderResourceView(device, img.GetImages(), img.GetImageCount(), md, resourceView);
  }
#endif

  if (FAILED(result))
  {
    printf("There was a problem loading \"%s\"\n", filename);
  }

	return result;
}