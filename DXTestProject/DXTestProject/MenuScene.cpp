#include "stdafx.h"
#include "MenuScene.h"
#include "NewGameEngine.h"
#include "resource.h"

INT_PTR CALLBACK NewGameDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Initialize()
{
}

void MenuScene::OnEnter()
{
  // Send a message to launch our dialog here.
  // Once it's open, if we've created an event for clicking it, we can launch
  // the game screen.

  // Refer to how the About dialog is created and managed in the DXTestProject.cpp
}

void MenuScene::OnExit()
{
}

void MenuScene::Update(float dt)
{
}

void MenuScene::Render()
{
}

// Message handler for new game dialog box.
INT_PTR CALLBACK NewGameDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    switch (LOWORD(wParam))
    {
    case IDNEWGAME:
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    case IDEXIT:
      {
        HWND mainWindow = GetParent(hDlg);
        EndDialog(hDlg, LOWORD(wParam));

        DestroyWindow(mainWindow);
        return (INT_PTR)TRUE;
      }
    }
  }
  return (INT_PTR)FALSE;
}