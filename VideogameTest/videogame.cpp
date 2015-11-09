#include <Windows.h>
#include <d3d11.h>
//#include <d3dx11.h>

#define APPLICATION_NAME	"VIDEOGAME_TEST"

#pragma comment(lib,"d3d11.lib")


LRESULT CreateContext(HWND &hWnd, int _widthScreen, int _heightScreen);

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

  // TODO Crear el contexto DIRECTX
  CreateContext( hWnd, WIDTH_APPLICATION, HEIGHT_APPLICATION);

  ShowWindow( hWnd, SW_SHOWDEFAULT );
  UpdateWindow( hWnd );
  MSG msg;
  ZeroMemory( &msg, sizeof(msg) );

  // A�adir en el while la condici�n de salida del programa de la aplicaci�n

  while( msg.message != WM_QUIT )
  {
    if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
    else
    {
       // Main loop: A�adir aqu� el Update y Render de la aplicaci�n principal
    }
  }
  UnregisterClass( APPLICATION_NAME, wc.hInstance );

  // A�adir una llamada a la alicaci�n para finalizar/liberar memoria de todos sus datos

  return 0;
}

LRESULT CreateContext(HWND &hWnd, int _widthScreen, int _heightScreen){
	// Tendremos que crear y rellenar una estructura de este tipo
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	// o
	//DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Windowed = TRUE;
	desc.BufferDesc.Width = _widthScreen;
	desc.BufferDesc.Height = _heightScreen;
	desc.BufferDesc.RefreshRate.Numerator = 1;
	desc.BufferDesc.RefreshRate.Denominator = 60;
	desc.OutputWindow = hWnd;
	desc.SampleDesc.Count = 1;
	//desc.SampleDesc.Quality;
	//desc. ????

	// Que DirectX queremos
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	ID3D11Device *l_D3DDevice = 0; // esta clase, el device, nos sirve para crear objetos de DirectX
	ID3D11DeviceContext *l_DeviceContext = 0; // el contexto nos va a servir para usar objetos de DirectX
	IDXGISwapChain *l_SwapChain = 0; // la cadena de swap

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &desc, &l_SwapChain, &l_D3DDevice, NULL, &l_DeviceContext)))
	{
		return S_FALSE;
	}
	return S_OK;
}