#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <dxgitype.h>
#include <d3dcompiler.h>

#ifdef OLD_DX_SDK
#include <xnamath.h>
#include <D3DX11tex.h>
#include <D3DX11async.h>

#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dxerr.lib")

#else
#include <DirectXMath.h>

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX;
#endif