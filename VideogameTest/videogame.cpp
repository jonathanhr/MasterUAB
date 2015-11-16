#include <Windows.h>
#include <d3d11.h>
//#include <d3dx11.h>
#include "ContextManager.h"
#include "Application.h"
#include "DebugRender.h"
#include "InputManagerImplementation.h"
#include "SphericalCameraController.h"

#define APPLICATION_NAME	"VIDEOGAME_TEST"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"winmm.lib")

CContextManager m_ContextManager;

float m_ElapsedTime = 0.f;
float m_PreviousTime = 0.f;

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

	switch( msg )
	{
		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}
		break;
		case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					//Cleanup();
				PostQuitMessage( 0 );
				return 0;
				break;
			}
		}
		break;
		case WM_SIZE:
			if (wParam != SIZE_MINIMIZED)
			{
				m_ContextManager.Resize(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
			}
			return 0;
	}//end switch( msg )

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
  // Register the window class
  WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

  RegisterClassEx( &wc );

  // Calcular el tamano de nuestra ventana
  RECT rc = {
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)
		};
  AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE);

  //int WIDTH_APPLICATION = rc.right - rc.left;
  //int HEIGHT_APPLICATION = rc.bottom - rc.top;
  int WIDTH_APPLICATION = 800;
  int HEIGHT_APPLICATION = 600;
  
  // Create the application's window
  HWND hWnd = CreateWindow( APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, rc.left, rc.top, WIDTH_APPLICATION, HEIGHT_APPLICATION, NULL, NULL, wc.hInstance, NULL);
  
  m_ContextManager.CreateContext( hWnd, WIDTH_APPLICATION, HEIGHT_APPLICATION);

  ShowWindow( hWnd, SW_SHOWDEFAULT );
  
  m_ContextManager.CreateBackBuffer(hWnd,WIDTH_APPLICATION,HEIGHT_APPLICATION);
  m_ContextManager.InitStates();
  CDebugRender debugRender(m_ContextManager.GetDevice());
  CSphericalCameraController m_CameraController;

  CApplication m_Application(&debugRender, &m_ContextManager, &m_CameraController);

  CInputManagerImplementation inputManager;
  CInputManager::SetCurrentInputManager(&inputManager);
  inputManager.LoadCommandsFromFile("Data\\input.xml");

  UpdateWindow( hWnd );
  MSG msg;
  ZeroMemory( &msg, sizeof(msg) );

  // Añadir en el while la condición de salida del programa de la aplicación

  while( msg.message != WM_QUIT )
  {	  
	  if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	  {
		switch (msg.message)
		{
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
			if (!inputManager.KeyEventReceived(msg.wParam, msg.lParam))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			break;
		default:
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}
	else
		{
		inputManager.BeginFrame();
		inputManager.EndFrame();
		// Main loop: Añadir aquí el Update y Render de la aplicación principal
		DWORD l_CurrentTime = timeGetTime();
		m_ElapsedTime = (float)(l_CurrentTime - m_PreviousTime)*0.001f;
		m_PreviousTime = l_CurrentTime;

		m_Application.Update(m_ElapsedTime);
		m_Application.Render();
    }
  }
  UnregisterClass( APPLICATION_NAME, wc.hInstance );

  // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

  return 0;
}