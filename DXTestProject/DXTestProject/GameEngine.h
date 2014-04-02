#pragma once
#include "windows.h"

class SceneManager;
class GraphicsManager;

class GameEngine
{
public:
	virtual ~GameEngine(void);

	static GameEngine* GetInstance();
	static void Shutdown();

	SceneManager* GetSceneManager();
	GraphicsManager* GetGraphicsManager();

	bool IsInitialized();
	bool Initialize(HWND hWnd, HINSTANCE hInstance);

	void Update(float dt);
	void Paint();

private:
	GameEngine(void);
	static GameEngine *mEngineInstance;
	bool mIsInitialized;

  void CreateConsoleWindow();

	SceneManager *mSceneManager;
	GraphicsManager *mGraphicsManager;
};

