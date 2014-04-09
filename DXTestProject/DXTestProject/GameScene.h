#pragma once

#include "Scene.h"
#include "Lights.h"
#include "CollisionMesh.h"

class Player;
class Obstacle;
class Model;

class GameScene: public Scene
{
public:
  GameScene();
  ~GameScene();

	void Initialize();
	void Cleanup();

	void Update(float dt);
	void Paint();

  void OnEnter();
  void OnExit();

  void HandleInput(float dt);
  void ViewSizeChanged();

protected:
	Obstacle *mObstacle;
  Player *mPlayer;
  float mDT;
  int mScore;

  Model *mLightModel;
  PointLight mLight;

  std::vector<CollisionMesh> mCollidableObjects;
  bool CheckCollisions();
};