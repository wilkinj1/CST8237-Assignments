#pragma once

#include "Scene.h"
#include "Lights.h"
#include "CollisionMesh.h"

class Player;
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

  void HandleInput(UINT wParam, UINT lParam);

protected:
	Player *mPlayer;
  Player *mSatellitePlayer;
  float mDT;
  int mScore;

  Model *mLightModel;
  PointLight mLight;

  std::vector<CollisionMesh> mCollidableObjects;
  void CheckCollisions();
};