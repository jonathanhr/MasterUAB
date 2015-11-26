#ifndef _EFFECT_PIXEL_SHADER
#define _EFFECT_PIXEL_SHADER

#pragma once

#include "EffectShader.h"

class CEffectPixelShader : public CEffectShader
{
protected:
	ID3D11PixelShader *m_PixelShader;

	void Destroy();
public:
	CEffectPixelShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectPixelShader();
	bool Load();
	//UAB_GET_PROPERTY(ID3D11PixelShader, PixelShader);
	//UAB_GET_PROPERTY(ID3D11Buffer, ConstantBuffer);
};

#endif