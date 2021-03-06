#include "ContextManager.h"

#include "Render\VertexTypes.h"
#include "Render\RenderableVertexs.h"
#include "Effect.h"

#include "Math\Matrix44.h"

#include "Render\DebugRender.h"

#pragma comment(lib,"d3d11.lib")


CContextManager::CContextManager()
	: m_D3DDevice(nullptr)
	, m_DeviceContext(nullptr)
	, m_SwapChain(nullptr)
	, m_RenderTargetView(nullptr)
	, m_DepthStencil(nullptr)
	, m_DepthStencilView(nullptr)
	, m_D3DDebug(nullptr)
{

	for (int i = 0; i < RS_COUNT; ++i)
	{
		m_RasterizerSates[i] = nullptr;
	}
	for (int i = 0; i < DSS_COUNT; ++i)
	{
		m_DepthStencilStates[i] = nullptr;
	}
	for (int i = 0; i < BLEND_COUNT; ++i)
	{
		m_BlendStates[i] = nullptr;
	}

}


CContextManager::~CContextManager()
{
	Dispose();
}

void CContextManager::Dispose()
{
	for (int i = 0; i < RS_COUNT; ++i)
	{
		CHECKED_RELEASE(m_RasterizerSates[i]);
	}
	for (int i = 0; i < DSS_COUNT; ++i)
	{
		CHECKED_RELEASE(m_DepthStencilStates[i]);
	}
	for (int i = 0; i < BLEND_COUNT; ++i)
	{
		CHECKED_RELEASE(m_BlendStates[i]);
	}

	CHECKED_RELEASE(m_RenderTargetView);
	CHECKED_RELEASE(m_DepthStencil);
	CHECKED_RELEASE(m_DepthStencilView);

	if (m_SwapChain)
	{
		m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		m_SwapChain->SetFullscreenState(FALSE, nullptr);
	}

	if (m_D3DDebug)
		m_D3DDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	CHECKED_RELEASE(m_D3DDebug);

	CHECKED_RELEASE(m_D3DDevice);
	CHECKED_RELEASE(m_DeviceContext);
	CHECKED_RELEASE(m_SwapChain);
}



HRESULT CContextManager::CreateContext(HWND hWnd, int Width, int Height)
{

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Width;
	sd.BufferDesc.Height = Height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

#if _DEBUG
	//int flags = D3D11_CREATE_DEVICE_DEBUG;
	int flags = 0;
#else
	int flags = 0;
#endif
	
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_D3DDevice, NULL, &m_DeviceContext)))
	{
		return S_FALSE;
	}

/*#if _DEBUG
	HRESULT hr = m_D3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_D3DDebug));
	if (FAILED(hr))
		return hr;
#endif*/


	// treure el ALT+INTRO autom�tic
	IDXGIFactory* dxgiFactory;
	HRESULT hr;
	hr = m_SwapChain->GetParent(__uuidof(IDXGIFactory), (void **)&dxgiFactory);
	assert(hr == S_OK);

	hr = dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
	assert(hr == S_OK);

	dxgiFactory->Release();

	return S_OK;
}

HRESULT CContextManager::CreateBackBuffer(HWND hWnd, int Width, int Height)
{

	CHECKED_RELEASE(m_RenderTargetView);
	CHECKED_RELEASE(m_DepthStencil);
	CHECKED_RELEASE(m_DepthStencilView);

	m_Width = Width;
	m_Height = Height;

	ID3D11Texture2D *pBackBuffer;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return FALSE;
	HRESULT hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return FALSE;


	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = Width;
	descDepth.Height = Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_D3DDevice->CreateTexture2D(&descDepth, NULL, &m_DepthStencil);
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_D3DDevice->CreateDepthStencilView(m_DepthStencil, &descDSV, &m_DepthStencilView);
	if (FAILED(hr))
		return hr;

	return S_OK;
}



class CDebugCEffect : public CEffect
{
public:
	bool Load(ID3D11Device *Device)
	{
		const char vs[] = {
			0x44, 0x58, 0x42, 0x43, 0x34, 0x4f, 0xb6, 0xe4, 0xbc, 0xb2, 0xdf, 0x47, 0x4e, 0x87, 0x8b, 0x7d, 0x80, 0xfc, 0x58, 0x1d, 0x01, 0x00, 0x00, 0x00, 0x60, 0x05, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x34
			, 0x00, 0x00, 0x00, 0x0c, 0x02, 0x00, 0x00, 0x5c, 0x02, 0x00, 0x00, 0xb0, 0x02, 0x00, 0x00, 0xe4, 0x04, 0x00, 0x00, 0x52, 0x44, 0x45, 0x46, 0xd0, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4c
			, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x04, 0xfe, 0xff, 0x00, 0x01, 0x00, 0x00, 0x9c, 0x01, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x6f, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x74, 0x42
			, 0x75, 0x66, 0x66, 0x65, 0x72, 0x00, 0xab, 0x3c, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c
			, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x01, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40
			, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x14
			, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x01, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54
			, 0x01, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x10
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x01, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x8c
			, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x5f, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x00, 0x03, 0x00, 0x03, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d
			, 0x5f, 0x56, 0x69, 0x65, 0x77, 0x00, 0x6d, 0x5f, 0x50, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x6d, 0x5f, 0x42, 0x61, 0x73, 0x65, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x01
			, 0x00, 0x03, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x5f, 0x43, 0x61, 0x6d, 0x65, 0x72, 0x61, 0x52, 0x69, 0x67, 0x68, 0x74, 0x56, 0x65, 0x63, 0x74
			, 0x6f, 0x72, 0x00, 0x6d, 0x5f, 0x43, 0x61, 0x6d, 0x65, 0x72, 0x61, 0x55, 0x70, 0x56, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x00, 0x6d, 0x5f, 0x44, 0x65, 0x62, 0x75, 0x67, 0x52, 0x65, 0x6e, 0x64, 0x65
			, 0x72, 0x53, 0x63, 0x61, 0x6c, 0x65, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x69, 0x63, 0x72, 0x6f, 0x73, 0x6f, 0x66, 0x74
			, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4c, 0x53, 0x4c, 0x20, 0x53, 0x68, 0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x72, 0x20, 0x36, 0x2e, 0x33, 0x2e, 0x39, 0x36
			, 0x30, 0x30, 0x2e, 0x31, 0x36, 0x33, 0x38, 0x34, 0x00, 0xab, 0xab, 0x49, 0x53, 0x47, 0x4e, 0x48, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x07, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03
			, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0xab, 0x4f, 0x53, 0x47, 0x4e, 0x4c
			, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f
			, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x53, 0x56, 0x5f, 0x50, 0x4f
			, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0xab, 0xab, 0x53, 0x48, 0x44, 0x52, 0x2c, 0x02, 0x00, 0x00, 0x40, 0x00, 0x01, 0x00, 0x8b, 0x00, 0x00, 0x00, 0x59
			, 0x00, 0x00, 0x04, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x03, 0x72, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5f, 0x00, 0x00, 0x03, 0xf2
			, 0x10, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x04, 0xf2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x01
			, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x08, 0x72, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x12, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06
			, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0x82, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x11
			, 0x00, 0x00, 0x08, 0x12, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x22, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x42, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x82, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x12, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x22, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x42, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x82, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x12, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x22, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x42, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x11
			, 0x00, 0x00, 0x08, 0x82, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x0e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x38
			, 0x00, 0x00, 0x08, 0xf2, 0x20, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x1e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x46, 0x8e, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x3e
			, 0x00, 0x00, 0x01, 0x53, 0x54, 0x41, 0x54, 0x74, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		const int vs_size = 1376;

		const char ps[] = {
			0x44, 0x58, 0x42, 0x43, 0x21, 0x6f, 0x65, 0x10, 0x36, 0xef, 0xfb, 0xc2, 0x96, 0x87, 0xee, 0x91, 0xfe, 0xb7, 0x7e, 0x15, 0x01, 0x00, 0x00, 0x00, 0xd0, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x34
			, 0x00, 0x00, 0x00, 0x8c, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x14, 0x01, 0x00, 0x00, 0x54, 0x01, 0x00, 0x00, 0x52, 0x44, 0x45, 0x46, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x04, 0xff, 0xff, 0x00, 0x01, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x4d, 0x69, 0x63, 0x72, 0x6f, 0x73, 0x6f, 0x66, 0x74
			, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4c, 0x53, 0x4c, 0x20, 0x53, 0x68, 0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x72, 0x20, 0x36, 0x2e, 0x33, 0x2e, 0x39, 0x36
			, 0x30, 0x30, 0x2e, 0x31, 0x36, 0x33, 0x38, 0x34, 0x00, 0xab, 0xab, 0x49, 0x53, 0x47, 0x4e, 0x4c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03
			, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x53, 0x56, 0x5f, 0x50, 0x4f, 0x53, 0x49, 0x54, 0x49, 0x4f, 0x4e, 0x00, 0x43, 0x4f, 0x4c, 0x4f, 0x52, 0x00, 0xab, 0xab, 0x4f
			, 0x53, 0x47, 0x4e, 0x2c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x53, 0x56, 0x5f, 0x54, 0x61, 0x72, 0x67, 0x65, 0x74, 0x00, 0xab, 0xab, 0x53, 0x48, 0x44, 0x52, 0x38, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x0e
			, 0x00, 0x00, 0x00, 0x62, 0x10, 0x00, 0x03, 0xf2, 0x10, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x03, 0xf2, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x05, 0xf2
			, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x1e, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x01, 0x53, 0x54, 0x41, 0x54, 0x74, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		const int ps_size = 464;



		HRESULT l_HR = Device->CreateVertexShader(vs, vs_size, NULL, &m_VertexShader);
		assert(l_HR == S_OK);

		D3D11_INPUT_ELEMENT_DESC l_Layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		l_HR = Device->CreateInputLayout(l_Layout, 2, vs, vs_size, &m_VertexLayout);
		assert(l_HR == S_OK);


		l_HR = Device->CreatePixelShader(ps, ps_size, NULL, &m_PixelShader);
		assert(l_HR == S_OK);


		D3D11_BUFFER_DESC l_BufferDescription = {};

		l_BufferDescription.Usage = D3D11_USAGE_DEFAULT;
		l_BufferDescription.ByteWidth = sizeof(CEffectParameters);
		l_BufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		l_BufferDescription.CPUAccessFlags = 0;
		assert(sizeof(CEffectParameters) % 16 == 0 && "CEffectParameters' size must be a multiple of 16!!");
		l_HR = Device->CreateBuffer(&l_BufferDescription, nullptr, &m_ConstantBuffer);
		assert(l_HR == S_OK);

		return true;
	}
};

CEffect *s_DebugEffect;

void CContextManager::InitStates()
{
	CDebugCEffect *l_DebugEffect = new CDebugCEffect();
	l_DebugEffect->Load(m_D3DDevice);
	s_DebugEffect = l_DebugEffect;

	InitRasterizerStates();
	InitDepthStencilStates();
	InitBlendStates();
}

void CContextManager::InitRasterizerStates()
{
	{
		D3D11_RASTERIZER_DESC l_WireframeDesc;
		ZeroMemory(&l_WireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		l_WireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		l_WireframeDesc.CullMode = D3D11_CULL_NONE;
		l_WireframeDesc.FrontCounterClockwise = true;

		HRESULT l_HR = m_D3DDevice->CreateRasterizerState(&l_WireframeDesc, &m_RasterizerSates[RS_WIREFRAME]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_RASTERIZER_DESC l_WireframeDesc;
		ZeroMemory(&l_WireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		l_WireframeDesc.FillMode = D3D11_FILL_SOLID;
		l_WireframeDesc.CullMode = D3D11_CULL_NONE;
		l_WireframeDesc.FrontCounterClockwise = true;

		HRESULT l_HR = m_D3DDevice->CreateRasterizerState(&l_WireframeDesc, &m_RasterizerSates[RS_SOLID]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_RASTERIZER_DESC l_WireframeDesc;
		ZeroMemory(&l_WireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		l_WireframeDesc.FillMode = D3D11_FILL_SOLID;
		l_WireframeDesc.CullMode = D3D11_CULL_FRONT;
		l_WireframeDesc.FrontCounterClockwise = false;

		HRESULT l_HR = m_D3DDevice->CreateRasterizerState(&l_WireframeDesc, &m_RasterizerSates[RS_FRONT_CULL]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_RASTERIZER_DESC l_WireframeDesc;
		ZeroMemory(&l_WireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		l_WireframeDesc.FillMode = D3D11_FILL_SOLID;
		l_WireframeDesc.CullMode = D3D11_CULL_BACK;
		l_WireframeDesc.FrontCounterClockwise = false;

		HRESULT l_HR = m_D3DDevice->CreateRasterizerState(&l_WireframeDesc, &m_RasterizerSates[RS_BACK_CULL]);
		assert(l_HR == S_OK);
	}
}

void CContextManager::InitDepthStencilStates()
{
	{
		D3D11_DEPTH_STENCIL_DESC l_desc = {};
		l_desc.DepthEnable = true;		
		l_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		l_desc.DepthFunc = D3D11_COMPARISON_LESS;

		HRESULT l_HR = m_D3DDevice->CreateDepthStencilState(&l_desc, &m_DepthStencilStates[DSS_DEPTH_ON]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_DEPTH_STENCIL_DESC l_desc = {};
		l_desc.DepthEnable = false;
		l_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		l_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT l_HR = m_D3DDevice->CreateDepthStencilState(&l_desc, &m_DepthStencilStates[DSS_OFF]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_DEPTH_STENCIL_DESC l_desc = {};
		l_desc.DepthEnable = true;
		l_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		l_desc.DepthFunc = D3D11_COMPARISON_LESS;
		// Stencil test parameters
		l_desc.StencilEnable = true;
		l_desc.StencilReadMask = 0xFF;
		l_desc.StencilWriteMask = 0xFF;
		// Stencil operations if pixel is front-facing
		l_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		l_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		l_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		l_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// Stencil operations if pixel is back-facing
		l_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		l_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		l_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		l_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT l_HR = m_D3DDevice->CreateDepthStencilState(&l_desc, &m_DepthStencilStates[DSS_DEPTH_TEST]);
		assert(l_HR == S_OK);
	}

}

void CContextManager::InitBlendStates()
{
	{
		D3D11_BLEND_DESC l_desc = {};
		l_desc.RenderTarget[0].BlendEnable = false;
		l_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		l_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		l_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		l_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		l_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT l_HR = m_D3DDevice->CreateBlendState(&l_desc, &m_BlendStates[BLEND_SOLID]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_BLEND_DESC l_desc = {};
		l_desc.RenderTarget[0].BlendEnable = true;
		l_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		l_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		l_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		l_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		l_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT l_HR = m_D3DDevice->CreateBlendState(&l_desc, &m_BlendStates[BLEND_CLASSIC]);
		assert(l_HR == S_OK);
	}

	{
		D3D11_BLEND_DESC l_desc = {};
		l_desc.RenderTarget[0].BlendEnable = true;
		l_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		l_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		l_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		l_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		l_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		l_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT l_HR = m_D3DDevice->CreateBlendState(&l_desc, &m_BlendStates[BLEND_PREMULT]);
		assert(l_HR == S_OK);
	}
}

void CContextManager::Resize(HWND hWnd, unsigned int Width, unsigned int Height)
{
	if (m_D3DDevice != nullptr)
	{
		m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		CHECKED_RELEASE(m_RenderTargetView);
		CHECKED_RELEASE(m_DepthStencil);
		CHECKED_RELEASE(m_DepthStencilView);

		m_SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0);
		HRESULT hr = CreateBackBuffer(hWnd, Width, Height);
		assert(hr == S_OK);
	}
}

void CContextManager::Draw(const CRenderableVertexs* _VerticesToRender, ERasterizedState _RS, EDepthStencilStates _DSS, EBlendStates _BS)
{
	m_DeviceContext->RSSetState(m_RasterizerSates[_RS]);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilStates[_DSS], 0);
	Vect4f v(1, 1, 1, 1);
	m_DeviceContext->OMSetBlendState(m_BlendStates[_BS], &v.x, 0xffffffff);

	s_DebugEffect->UpdateParameters(m_DeviceContext, &m_Parameters);
	s_DebugEffect->SetActive(m_DeviceContext);

	_VerticesToRender->SetBuffers(m_DeviceContext);

	_VerticesToRender->Draw(m_DeviceContext);
}

void CContextManager::BeginRender(CColor backgroundColor)
{
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)m_Width;
	vp.Height = (FLOAT)m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);

	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, &backgroundColor.x);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
}

void CContextManager::EndRender()
{
	m_SwapChain->Present(0, 0);
}
