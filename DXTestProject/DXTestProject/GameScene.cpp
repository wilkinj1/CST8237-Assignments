#include "stdafx.h"
#include "GameScene.h"
#include "Player.h"

GameScene::GameScene()
{
	mPlayer = new Player();
}

GameScene::~GameScene()
{
  delete mPlayer;
}

void GameScene::Initialize()
{
  mPlayer->Initialize();
}

void GameScene::OnEnter()
{
}

void GameScene::OnExit()
{
}

void GameScene::Update(float dt)
{
  mPlayer->Update(dt);
}

void GameScene::Render()
{
  mPlayer->Render();
}