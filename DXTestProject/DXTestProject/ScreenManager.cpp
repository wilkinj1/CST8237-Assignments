#include "stdafx.h"
#include "SceneManager.h"
#include "GameScene.h"

SceneManager::SceneManager(void) { }

SceneManager::~SceneManager(void) 
{
  PopAllScenes();
}

void SceneManager::PushScene(GameScene *Scene)
{
  if(!mScenes.empty())
  {
    mScenes.back()->OnExit();
  }

  mScenes.push_back(Scene);
  Scene->OnEnter();
}

GameScene* SceneManager::PopScene()
{
  GameScene *SceneToPop = NULL;

  if(!mScenes.empty())
  {
    SceneToPop = mScenes.back();
    SceneToPop->OnExit();
    mScenes.pop_back();

    if(!mScenes.empty())
    {
      mScenes.back()->OnEnter();
    }
  }

  return SceneToPop;
}

void SceneManager::Update(float dt)
{
	if(!mScenes.empty())
	{
		mScenes.back()->Update(dt);
	}
}
	
void SceneManager::Paint()
{
	if(!mScenes.empty())
	{
		mScenes.back()->Paint();
	}
}

GameScene* SceneManager::GetActiveScene()
{
  GameScene *SceneToReturn = NULL;
  if(!mScenes.empty())
  {
    SceneToReturn = mScenes.back();
  }

  return SceneToReturn;
}

void SceneManager::PopAllScenes()
{
  while(!mScenes.empty())
  {
    PopScene();
  }
}