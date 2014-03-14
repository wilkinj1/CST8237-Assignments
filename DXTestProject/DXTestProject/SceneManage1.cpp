#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"

void SceneManager::Initialize()
{

}

void SceneManager::Update(float dt)
{
  if(!mScenes.empty())
  {
    Scene *currentScene = mScenes.back();
    currentScene->Update(dt);
  }
}

void SceneManager::Render()
{
  if(!mScenes.empty())
  {
    Scene *currentScene = mScenes.back();
    currentScene->Render();
  }
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

void SceneManager::PopScene()
{
  // Do a check to see if we're empty.
  if(!mScenes.empty())
  {
    mScenes.back()->OnExit();
    mScenes.pop_back();

    // If we're still not empty after popping the last element off..
    if(!mScenes.empty())
    {
      mScenes.back()->OnEnter();
    }
  }
}

  void SceneManager::PopAllScenes()
  {
    while(!mScenes.empty())
    {
      PopScene();
    }
  }