#pragma once

#include "Scene.h"

class Player;

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
  float mDT;
  int mScore;
};