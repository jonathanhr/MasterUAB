#include "ContextManager.h"

CContextManager::CContextManager(void)
{
	l_D3DDevice = 0;
	l_DeviceContext = 0;
	l_SwapChain = 0;
	l_RenderTargetView = 0;
	l_DepthStencil = 0;
	l_DepthStencilView = 0;
}

CContextManager::~CContextManager(void)
{
	l_D3DDevice->Release();
	l_DeviceContext->Release();
	l_SwapChain->Release();
	l_RenderTargetView->Release();
	l_DepthStencil->Release();
	l_DepthStencilView->Release();
}

ID3D11Device * CContextManager::getDevice() const{
	return l_D3DDevice;
}

ID3D11DeviceContext * CContextManager::getContext() const{
	return l_DeviceContext;
}

IDXGISwapChain * CContextManager::getSwap() const{
	return l_SwapChain;
}

LRESULT CContextManager::CreateContext(HWND &hWnd, int _widthScreen, int _heightScreen){
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

	// Que DirectX queremos
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &desc, &l_SwapChain, &l_D3DDevice, NULL, &l_DeviceContext)))
	{
		return S_FALSE;
	}

	return S_OK;
}

LRESULT CContextManager::BackBuffer(){
	ID3D11Texture2D *pBackBuffer;
	if (FAILED(l_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return FALSE;
	HRESULT hr = l_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &l_RenderTargetView);
	pBackBuffer->Release();

	return hr;
}

LRESULT CContextManager::getDepth(){
	HRESULT hr = getDepth2(l_DepthStencil);
	hr = getDepth3(l_DepthStencil,l_DepthStencilView);
	return hr;
}

LRESULT CContextManager::getDepth2(ID3D11Texture2D *_l_DepthStencil){
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	// TODO:
	//desc.Width;
	//desc.Height;
	//desc.MipLevels;   <- 1
	//desc.ArraySize;   <- 1
	//desc.SampleDesc.Count;
	//desc.SampleDesc.Quality;
	HRESULT hr = l_D3DDevice->CreateTexture2D(&desc, NULL, &_l_DepthStencil);
	if (FAILED(hr))
		return hr;
}

LRESULT CContextManager::getDepth3(ID3D11Texture2D *_l_DepthStencil, ID3D11DepthStencilView *l_DepthStencilView){
	D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;
	// TODO
	//desc.Format;
	HRESULT hr = l_D3DDevice->CreateDepthStencilView(_l_DepthStencil, &desc, &l_DepthStencilView);
	if (FAILED(hr))
		return hr;
}

void CContextManager::Render(){
	// Limpiar el framebuffer:
	float color[4] = {1,0,1,1};
	l_DeviceContext->ClearRenderTargetView(l_RenderTargetView, color);
	l_DeviceContext->ClearDepthStencilView(l_DepthStencilView,1,1,1);

	// definir a que target vamos a pintar:
	l_DeviceContext->OMSetRenderTargets(1, &l_RenderTargetView, nullptr);

	// definir el viewport:
	D3D11_VIEWPORT vp[1];
    vp[0].Width = 800;
    vp[0].Height = 600;
    vp[0].MinDepth = 0;
    vp[0].MaxDepth = 1;
    vp[0].TopLeftX = 0;
    vp[0].TopLeftY = 0;
	l_DeviceContext->RSSetViewports(1,vp);

	// TODO renderizar aqui
	Draw();

	// presentar lo pintado:
	l_SwapChain->Present(0,0);
}

void CContextManager::Draw(){
	
}