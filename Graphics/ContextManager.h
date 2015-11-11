#ifndef _CONTEXT_MANAGER
#define _CONTEXT_MANAGER

#pragma once

#include <d3d11.h>

class CContextManager
{
private:
	ID3D11Device *l_D3DDevice; // esta clase, el device, nos sirve para crear objetos de DirectX
	ID3D11DeviceContext *l_DeviceContext; // el contexto nos va a servir para usar objetos de DirectX
	IDXGISwapChain *l_SwapChain; // la cadena de swap
	ID3D11RenderTargetView*	l_RenderTargetView;
	ID3D11Texture2D* l_DepthStencil;
	ID3D11DepthStencilView*	l_DepthStencilView;
	int WIDTH_APPLICATION;
	int HEIGHT_APPLICATION;

public:
	CContextManager(void);
	~CContextManager(void);

	void setWidthScreen(int _width);
	void setHeightScreen(int _height);
	int getWidthScreen();
	int getHeightScreen();
	
	ID3D11Device * getDevice() const;
	ID3D11DeviceContext * getContext() const;
	IDXGISwapChain * getSwap() const;

	LRESULT CreateContext(HWND &hWnd, int _widthScreen, int _heightScreen);
	LRESULT BackBuffer();
	void Draw();
	LRESULT getDepth();
	LRESULT getDepth2(ID3D11Texture2D *_l_DepthStencil);
	LRESULT getDepth3(ID3D11Texture2D *_l_DepthStencil, ID3D11DepthStencilView *l_DepthStencilView);

	void Render();	
};

#endif

