gameprogrammingpatterns.com/service_locator.html

// *****************************************   ContextManager.cpp

// TODO:
/**		1 - Hacer la app compatible con cambiar el tamaño

		1.1 - Corregir "CContextManager::CreateBackBuffer"
            - Guardar el Width y Height que se pasan por parámetro.
            - Mover el "RSSetViewport" y el "OMSetRenderTargets" a "CContextManager::BeginRender"
			- Eliminar el código de creación de RasterizerState ya que es redundante.
			- Crear la función "Resize"
			- Asegurarse de eliminar "m_RenderTargetView", "m_DepthStencil" y "m_DepthStencilView" 
				al principio de la función
			
		1.2 - Capturar el cambio de tamaño
			- En "videogame.cpp" mover el "CContextManager" a variable global.
			- Llamar dicha función desde MsgProc cuando llegue la orden de resize
**/

void CContextManager::Resize(HWND hWnd, unsigned int Width, unsigned int Height)
{
	if (m_D3DDevice != nullptr)
	{
		CHECKED_RELEASE(m_RenderTargetView);
		CHECKED_RELEASE(m_DepthStencil);
		CHECKED_RELEASE(m_DepthStencilView);
		
		m_SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
		HRESULT hr = CreateBackBuffer(hWnd, Width, Height);
		assert(hr == S_OK);
	}
}

{
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			s_Context.Resize(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
		}
		return 0;
}

// TODO:
/**
		2. - Añadir el InputManager
		
		2.1 - Copiar InputManager e InputManagerImplementation al proyecto "Core"
		
		2.2 - Asegurarse de que compile correctamente
		
		2.3 - Rellenar los TODO 2.3
		
		2.4 - Configurar el inputmanager
			- Crear el input manager en videogame.cpp
			- Llamar CInputManager::SetCurrentInputManager(&inputManager);
			- inputManager.LoadCommandsFromFile("Data\\input.xml");
			- Actualizar el PeekMessage
			- llamar "inputManager.BeginFrame()" y "inputManager.EndFrame()" 
				en la parte principal del bucle
			- eliminar los "WM_SYSKEYDOWN, WM_SYSKEYUP, WM_KEYDOWN, WM_KEYUP" 
				que haya en MsgProc
		
		2.5 - Añadir el XML de configuración
			- Añadir los include path 
				- "$(SolutionDir)3rdParty\libxml2-2.7.2.win32\include\"
				- "$(SolutionDir)3rdParty\iconv-1.9.2.win32\include\"
			- Añadir los lib path 
				- "$(SolutionDir)3rdParty\libxml2-2.7.2.win32\lib\"
				- "$(SolutionDir)3rdParty\iconv-1.9.2.win32\lib\"
			- Añadir la libreria libxml2.lib
			- Copiar las dll al directorio dónde se ejecuta el juego
				- libxml2.dll
				- zlib1.dll
				- iconv.dll
			- Copiar Data\input.xml
**/

// TODO 2.4.1:
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


// TODO:
/**
		3 - Añadir una cámara
		
		3.1 - Corregir CameraController.cpp
			- Añadir el inicializador m_Position(0,0,0)
			
		3.2 - Añadir "CSphericalCameraController" en CApplication
		
		3.3 - En el cpp implementar la funcionalidad de la camara
			- Actualizar el update
			- actualizar el render con: m_Camera.SetCamera(&camera)
		
**/

// 3.3
{
	Vect3f cameraMovement(0,0,0);

	if (CInputManager::GetInputManager()->IsActionActive("MOVE_LEFT"))
	{
		cameraMovement.x += 0.01f * _ElapsedTime;
	}
	if (CInputManager::GetInputManager()->IsActionActive("MOVE_RIGHT"))
	{
		cameraMovement.x -= 0.01f * _ElapsedTime;
	}
	if (CInputManager::GetInputManager()->IsActionActive("MOVE_UP"))
	{
		cameraMovement.y += 0.01f * _ElapsedTime;
	}
	if (CInputManager::GetInputManager()->IsActionActive("MOVE_DOWN"))
	{
		cameraMovement.y -= 0.01f * _ElapsedTime;
	}

	m_Camera.Update(cameraMovement);
}

// TODO:
/**
		4 - Añadir AntTweakBar
		
		4.1 - Copiar el proyecto de Codi2 a 3rdParty
		
		4.2 - Añadir el proyecto a la solución
		
		4.3 - Pasarle los eventos

		4.4 - Vincular algunas variables
		
		4.5 - Pintar el TweakBar
		
		4.6 - Finalizar el TweakBar en el destructor
**/


// 4.3
{
	if (TwEventWin(hWnd, msg, wParam, lParam)) // send event message to AntTweakBar
		return 0; // event has been handled by AntTweakBar
}

// 4.4.1 - Añadir variables en CApplication
{
	TwBar *m_myBar;
	uint32_t m_BackgroundColor = 0xff00008b;
}
	
// 4.4.2 - En el init
{
	s_myBar = TwNewBar("NameOfMyTweakBar");
	
	TwAddVarRW(s_myBar, "background color", TW_TYPE_COLOR32, &m_BackgroundColor, " coloralpha=true colororder=argb ");
}

// 4.4.3
void CApplicationDX::BeginRenderDX()
{
	CColor backgroundColor; // (.2f, .1f, .4f);
	backgroundColor.SetUint32Argb(s_myColor);
	
	...

}

// 4.5
{
	TwDraw();
}

// 4.6
{
	TwTerminate();
}

// TODO: Opcional
/**
		O1 - Implementar fullscreen
				aka BLACK MAGIC
				
		O1.1 - Preparar el contexto para que sea compatible con el cambio
			- En el DXGI_SWAP_CHAIN_DESC, añadir el flag "DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH"
			- Desactivar el cambio automático tras crear el contexto  -> BLACK MAGIC 1
		
		O1.2 - Añadir en "videogame.cpp" la función para hacer el cambio -> BLACK MAGIC 2

		O1.3 - Añadir en el bucle principal la llamada a ésta funcionalidad
**/

{
	// BLACK MAGIC 1
	IDXGIFactory* dxgiFactory;
	hr = m_SwapChain->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory);
	assert(hr == S_OK);

	hr = dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
	assert(hr == S_OK);

	dxgiFactory->Release();
}

// BLACK MAGIC 2
void ToggleFullscreen(HWND Window, WINDOWPLACEMENT &WindowPosition)
{
	// This follows Raymond Chen's prescription
	// for fullscreen toggling, see:
	// http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx

	DWORD Style = GetWindowLongW(Window, GWL_STYLE);
	if (Style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO MonitorInfo = { sizeof(MonitorInfo) };
		if (GetWindowPlacement(Window, &WindowPosition) &&
			GetMonitorInfoW(MonitorFromWindow(Window, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
		{
			SetWindowLongW(Window, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(Window, HWND_TOP,
				MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
				MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
				MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else
	{
		SetWindowLongW(Window, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(Window, &WindowPosition);
		SetWindowPos(Window, 0, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}


// O1.3
{
	if (inputManager.IsActionActive("TOGLE_FULLCREEN"))
	  {
		  WINDOWPLACEMENT windowPosition = { sizeof(WINDOWPLACEMENT) };
		  GetWindowPlacement(msg.hwnd, &windowPosition);

		  ToggleFullscreen(msg.hwnd, windowPosition);
	  }
}
	
	
	
	
	
	
	
	
	
/**


**/

// TODO: OPCIONAL 2 añadir objeto de debug

ID3D11Debug*			m_D3DDebug;
		
{	
#if _DEBUG
	int flags = D3D11_CREATE_DEVICE_DEBUG;
#else
	int flags = 0;
#endif
	
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_D3DDevice, NULL, &m_DeviceContext)))
	{
		return S_FALSE;
	}

#if _DEBUG
	HRESULT hr = m_D3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_D3DDebug));
	if (FAILED(hr))
		return hr;
#endif
}

// en el destructor de CContextManager llamar a:
m_D3DDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);