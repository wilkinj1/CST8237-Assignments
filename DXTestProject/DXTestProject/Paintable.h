#pragma once

#ifdef OLD_DX_SDK
#include <xnamath.h>
#else
#include <DirectXMath.h>
using namespace DirectX;
#endif

struct ID3D11Device;
struct ID3D11DeviceContext;