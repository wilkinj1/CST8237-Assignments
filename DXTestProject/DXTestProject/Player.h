#pragma once

#include "GameObject.h"

#ifdef OLD_DX_SDK
#include <xnamath.h>
#else
#include <DirectXMath.h>
using namespace DirectX;
#endif

class Model;

class Player: public GameObject
{
public:
	Player();
	~Player();

	void Initialize();
	void Update(float dt);

  void Paint(const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &proj, const XMFLOAT4 &cameraPos, XMFLOAT4 lightPos);

protected:
  Model *mPlayerModel;
  Model *mSatelliteModel;
  Model *mLightModel;

  void Paint(const XMMATRIX &world, const XMMATRIX &viewProj, const XMFLOAT4 &cameraPos) { }
};