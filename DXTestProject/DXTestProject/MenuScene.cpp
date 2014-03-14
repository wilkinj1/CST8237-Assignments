#include "stdafx.h"
#include "MenuScene.h"
#include "GameScene.h"

#include "GameEngine.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "Resource.h"

INT_PTR CALLBACK NewGameDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

MenuScene::MenuScene() { }
MenuScene::~MenuScene() { }

void MenuScene::Initialize() { }
void MenuScene::Cleanup() { }

void MenuScene::Update(float dt) { }
void MenuScene::Paint() { }

void MenuScene::OnEnter() 
{
}
void MenuScene::OnExit() { }