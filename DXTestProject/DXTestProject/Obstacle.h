#pragma once

#include "GameObject.h"
#include "CollisionMesh.h"

class Model;
class Camera;

class Obstacle: public GameObject, public CollisionMeshDelegate
{
public:
	Obstacle();
	~Obstacle();

	void Initialize();
	void Update(float dt);

  void Paint(const XMMATRIX &world, Camera *camera, XMFLOAT3 lightPos);
  XMMATRIX GetWorldTransform();

  Model* GetModel();

protected:
  Model *mObstacleModel;

  void Paint(const XMMATRIX &world, const XMMATRIX &viewProj, const XMFLOAT4 &cameraPos) { }
  void CollisionOccurred(CollisionMeshDelegate *otherObject);
};