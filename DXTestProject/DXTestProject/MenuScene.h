#pragma once

#include "Scene.h"

class MenuScene: public Scene
{
public:
  MenuScene();
  ~MenuScene();

	void Initialize();
	void Cleanup();

	void Update(float dt);
	void Paint();

  void OnEnter();
  void OnExit();
};