#pragma once

#include "GameObject.h"

class Model;
class Camera;

class Player: public GameObject
{
public:
	Player();
	~Player();

	void Initialize();
	void Update(float dt);

  void Paint(const XMMATRIX &world, Camera *camera, XMFLOAT3 lightPos);

protected:
  Model *mPlayerModel;

  void Paint(const XMMATRIX &world, const XMMATRIX &viewProj, const XMFLOAT4 &cameraPos) { }
};