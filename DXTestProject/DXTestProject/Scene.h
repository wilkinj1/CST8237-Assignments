#pragma once

#include <vector>
#include "GraphicsIncludes.h"

class GameObject;
class Camera;
struct ID3D11DeviceContext;
struct ID3D11Device;

class Scene
{
public:
  Scene() : mIsInitialized(false) { }
  virtual ~Scene() { }

	virtual void Initialize() = 0;
	virtual void Update(float dt) = 0;
	virtual void Paint() = 0;

  virtual void HandleInput(UINT wParam, UINT lParam) { }

	virtual bool isInitialized()
	{
		return mIsInitialized;
	}

	Camera *GetSceneCamera() 
	{
		return mSceneCamera;
	}

  virtual void OnEnter() = 0;
  virtual void OnExit() = 0;

protected:
	std::vector<GameObject *> mGameObjects;
	bool mIsInitialized;
	Camera *mSceneCamera;
};