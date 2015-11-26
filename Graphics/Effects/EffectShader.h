#ifndef _EFFECT_SHADER
#define _EFFECT_SHADER

#pragma once

#include "Utils\Named.h"

#include <d3d11.h>

class CEffectShader : public CNamed
{
protected:
	ID3D11Buffer *m_ConstantBuffer;
	std::string m_Filename;
	std::string m_Preprocessor;
	std::string m_ShaderModel;
	std::string m_EntryPoint;

	virtual void Destroy();
	bool LoadShader(const std::string &Filename, const std::string &EntryPoint,
		const std::string &ShaderModel, ID3DBlob **BlobOut);
	bool CreateConstantBuffer();
public:
	CEffectShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectShader();
	virtual bool Load() = 0;
	virtual bool Reload();
};

#endif