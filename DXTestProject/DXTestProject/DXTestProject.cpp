// DXTestProject.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DXTestProject.h"
#include "NewGameEngine.h"
#include <d3d11.h>
#include <assert.h>
#include "SceneManager.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "GameTimer.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int, HWND&);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

GameScene *game = NULL;
MenuScene *menu = NULL;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPTSTR    lpCmdLine,
                       _In_ int       nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.
  MSG msg = { 0 };
  HACCEL hAccelTable;

  // Initialize global strings
  LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadString(hInstance, IDC_DXTESTPROJECT, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  HWND hWnd;

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow, hWnd))
  {
    return FALSE;
  }

  // intialize stuff
  NewGameEngine *engine = NewGameEngine::GetInstance();

  // If our engine didn't intialize properly, we're done.
  assert(engine->Initialize(hWnd));

  game = new GameScene();
  game->Initialize();

  menu = new MenuScene();
  menu->Initialize();

  engine->GetSceneManager()->PushScene(game);

  GameTimer timer;
  timer.Start();

  hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DXTESTPROJECT));

  // Main message loop:
  while( msg.message != WM_QUIT )
  {
	  /* We use PeekMessage instead of GetMessage because Peek isn't blocking, so that we can
	     perform any operation we want without having to wait for a Windows message to be received. */
    if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    else
    {
      if(!timer.IsStopped() && !timer.IsPaused())
      {
        // Tick our timer so that we can get the accurate time since the last call.
        timer.Tick();

        // do stuff
        engine->Update(timer.GetDeltaTime());
        engine->Render();
      }
    }
  };

  // Cleanup.
    engine->GetSceneManager()->PopAllScenes();
    delete game;
    game = NULL;

    delete menu;
    menu = NULL;

    NewGameEngine::DestroyInstance();

  return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style			= CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc	= WndProc;
  wcex.cbClsExtra		= 0;
  wcex.cbWndExtra		= 0;
  wcex.hInstance		= hInstance;
  wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DXTESTPROJECT));
  wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DXTESTPROJECT);
  wcex.lpszClassName	= szWindowClass;
  wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND& outHWnd)
{
  hInst = hInstance; // Store instance handle in our global variable

  outHWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

  if (!outHWnd)
  {
    return FALSE;
  }

  ShowWindow(outHWnd, nCmdShow);
  UpdateWindow(outHWnd);

  return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int wmId, wmEvent;
  PAINTSTRUCT ps;
  HDC hdc;

  switch (message)
  {
  case WM_COMMAND:
    wmId    = LOWORD(wParam);
    wmEvent = HIWORD(wParam);
    // Parse the menu selections:
    switch (wmId)
    {
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;
  case WM_SIZE:
    {
      RECT clientRect;
      GetClientRect(hWnd, &clientRect);

      float screenWidth = clientRect.right - clientRect.left;
      float screenHeight = clientRect.top - clientRect.bottom;

      // Update your camera's aspect ratio.
      float aspectRatio = screenWidth / screenHeight;
    }
    break;

  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    // TODO: Add any drawing code here...
    EndPaint(hWnd, &ps);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}
