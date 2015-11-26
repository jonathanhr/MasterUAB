#ifndef _EFFECT_VERTEX_SHADER
#define _EFFECT_VERTEX_SHADER

#pragma once

class CEffectVertexShader : public CEffectShader
{
protected:
	ID3D11VertexShader *m_VertexShader;
	ID3D11InputLayout *m_VertexLayout;
	std::string m_VertexType;
	void Destroy();
public:
	CEffectVertexShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectVertexShader();
	bool Load();
	//UAB_GET_PROPERTY(ID3D11VertexShader, VertexShader);
	//UAB_GET_PROPERTY(ID3D11InputLayout, VertexLayout);
	//UAB_GET_PROPERTY(ID3D11Buffer, ConstantBuffer);
};

#endif