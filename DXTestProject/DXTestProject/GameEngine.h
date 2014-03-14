#pragma once
#include "windows.h"

class SceneManager;
class GraphicsManager;

class GameEngine
{
public:
	virtual ~GameEngine(void);

	static GameEngine* getInstance();
	static void shutdown();

	SceneManager* getSceneManager();
	GraphicsManager* getGraphicsManager();

	bool isInitialized();
	bool Initialize(HWND hWnd, HINSTANCE hInstance);

	void Update(float dt);
	void Paint();

private:
	GameEngine(void);
	static GameEngine *mEngineInstance;
	bool mIsInitialized;

	SceneManager *mSceneManager;
	GraphicsManager *mGraphicsManager;
};

