#include "stdafx.h"
#include "SceneManager.h"


void SceneManager::Initialize()
{

}

void SceneManager::Update(float dt)
{

}

void SceneManager::Render()
{

}

void SceneManager::PushScene(Scene *scene)
{
	mScenes.push_back(scene);
}

void SceneManager::PopScene()
{
	mScenes.pop_back();
}