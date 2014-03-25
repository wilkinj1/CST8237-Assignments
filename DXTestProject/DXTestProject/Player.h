#pragma once

#include "GameObject.h"
#include "CollisionMesh.h"

class Model;
class Camera;

class Player: public GameObject, public CollisionMeshDelegate
{
public:
	Player();
	~Player();

	void Initialize();
	void Update(float dt);

  void Paint(const XMMATRIX &world, Camera *camera, XMFLOAT3 lightPos);
  XMMATRIX GetWorldTransform();

  Model* GetModel();

protected:
  Model *mPlayerModel;

  void Paint(const XMMATRIX &world, const XMMATRIX &viewProj, const XMFLOAT4 &cameraPos) { }
};