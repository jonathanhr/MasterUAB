#ifndef _EFFECT_TECHNIQUE
#define _EFFECT_TECHNIQUE

#pragma once

#include "Utils\Named.h"
#include "EffectVertexShader.h"
#include "EffectPixelShader.h"

class CEffectTechnique : public CNamed
{
private:
	CEffectVertexShader *m_VertexShader;
	CEffectPixelShader *m_PixelShader;
	std::string m_VertexShaderName;
	std::string m_PixelShaderName;
public:
	CEffectTechnique(CXMLTreeNode &TreeNode);
	virtual ~CEffectTechnique();
	//UAB_GET_PROPERTY(CEffectVertexShader, VertexShader);
	//UAB_GET_PROPERTY(CEffectPixelShader, PixelShader);
	void Refresh();
};

#endif