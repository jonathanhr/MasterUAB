#include <Windows.h>
#include <d3d11.h>
//#include <d3dx11.h>
#include "ContextManager.h"

#define APPLICATION_NAME	"VIDEOGAME_TEST"

#pragma comment(lib,"d3d11.lib")

CContextManager *m_ContextManager;

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

  int WIDTH_APPLICATION = rc.right - rc.left;
  int HEIGHT_APPLICATION = rc.bottom - rc.top;
  
  // Create the application's window
  HWND hWnd = CreateWindow( APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, rc.left, rc.top, WIDTH_APPLICATION, HEIGHT_APPLICATION, NULL, NULL, wc.hInstance, NULL);
  
  m_ContextManager = new CContextManager();

  // TODO Crear el contexto DIRECTX
  m_ContextManager->CreateContext( hWnd, WIDTH_APPLICATION, HEIGHT_APPLICATION);

  ShowWindow( hWnd, SW_SHOWDEFAULT );
  
  m_ContextManager->BackBuffer();

  UpdateWindow( hWnd );
  MSG msg;
  ZeroMemory( &msg, sizeof(msg) );

  // Añadir en el while la condición de salida del programa de la aplicación

  while( msg.message != WM_QUIT )
  {
    if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
    else
    {
       // Main loop: Añadir aquí el Update y Render de la aplicación principal
		m_ContextManager->Render();
    }
  }
  UnregisterClass( APPLICATION_NAME, wc.hInstance );

  // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

  return 0;
}