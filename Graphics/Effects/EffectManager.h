#ifndef _EFFECT_MANAGER
#define _EFFECT_MANAGER

#pragma once

#include "EffectTechnique.h"
#include "EffectParameters.h"

#include "Utils\TemplatedMapManager.h"

class CEffectManager : public CTemplatedMapManager<CEffectTechnique>
{
private:
	CTemplatedMapManager<CEffectVertexShader> m_VertexShaders;
	CTemplatedMapManager<CEffectPixelShader> m_PixelShaders;
public:
	static CEffectParameters m_Parameters;
	CEffectManager();
	virtual ~CEffectManager();

	void Reload();
	void Load(const std::string &Filename);
	CEffectVertexShader * GetVertexShader(const std::string &VertexShader);
	CEffectPixelShader * GetPixelShader(const std::string &PixelShader);
};

#endif