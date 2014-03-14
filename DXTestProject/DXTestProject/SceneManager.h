#pragma once

#include <vector>
#include "Paintable.h"

class Scene;
struct ID3D11DeviceContext;

class SceneManager
{
public:
	SceneManager(void);
	virtual ~SceneManager(void);

	void Update(float dt);
	void Paint();

  void PushScene(Scene *scene);
  Scene* PopScene();

  Scene* GetActiveScene();
  void PopAllScenes();

private:
	std::vector<Scene *> mScenes;
};
