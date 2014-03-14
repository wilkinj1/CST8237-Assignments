#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"

SceneManager::SceneManager(void) { }

SceneManager::~SceneManager(void) 
{
  PopAllScenes();
}

void SceneManager::PushScene(Scene *scene)
{
  if(!mScenes.empty())
  {
    mScenes.back()->OnExit();
  }

  mScenes.push_back(scene);
  scene->OnEnter();
}

Scene* SceneManager::PopScene()
{
  Scene *sceneToPop = NULL;

  if(!mScenes.empty())
  {
    sceneToPop = mScenes.back();
    sceneToPop->OnExit();
    mScenes.pop_back();

    if (!mScenes.empty())
    {
      mScenes.back()->OnEnter();
    }
  }

  return sceneToPop;
}

void SceneManager::Update(float dt)
{
  if (!mScenes.empty())
	{
    mScenes.back()->Update(dt);
	}
}
	
void SceneManager::Paint()
{
  if (!mScenes.empty())
	{
    mScenes.back()->Paint();
	}
}

Scene* SceneManager::GetActiveScene()
{
  Scene *sceneToReturn = NULL;
  if (!mScenes.empty())
  {
    sceneToReturn = mScenes.back();
  }

  return sceneToReturn;
}

void SceneManager::PopAllScenes()
{
  while (!mScenes.empty())
  {
    PopScene();
  }
}