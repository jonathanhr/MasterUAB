/*#include <string>

#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Math\Color.h"

#include "Render\RenderManager.h"

#define MV_VERTEX_TYPE_POSITION 0x0001
#define MV_VERTEX_TYPE_COLOR 0x0002
#define MV_VERTEX_TYPE_NORMAL 0x0004
#define MV_VERTEX_TYPE_BLEND_INDICES 0x0008
#define MV_VERTEX_TYPE_BLEND_WEIGHT 0x0010
#define MV_VERTEX_TYPE_TEXTURE1 0x0020
#define MV_VERTEX_TYPE_TEXTURE2 0x0040
#define MV_VERTEX_TYPE_POSITION4 0x0080

struct TCOLORED_VERTEX
{
	Vect3f x, y, z;
	CColor color;
	static bool CreateInputLayout(CRenderManager *RenderManager, ID3DBlob *VSBlob,
		ID3D11InputLayout **VertexLayout)
	{
		D3D11_INPUT_ELEMENT_DESC l_Layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
			D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT l_NumElements = ARRAYSIZE(l_Layout);
		HRESULT l_HR = RenderManager->GetDevice()->CreateInputLayout(l_Layout,
			l_NumElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), VertexLayout);
		return !FAILED(l_HR);
	}
	static unsigned int GetVertexType()
	{
		return MV_VERTEX_TYPE_POSITION | MV_VERTEX_TYPE_COLOR;
	}
};*/