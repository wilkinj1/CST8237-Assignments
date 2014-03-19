#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <dxgitype.h>
#include <d3dcompiler.h>

#ifdef OLD_DX_SDK
#include <xnamath.h>
#else
#include <DirectXMath.h>
using namespace DirectX;
#endif