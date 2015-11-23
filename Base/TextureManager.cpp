#include "TextureManager.h"

CTextureManager::CTextureManager(){}

CTextureManager::~CTextureManager(){
	Destroy();
}

CTexture * CTextureManager::GetTexture(const std::string &Filename){
	TMapResource::iterator it = m_Resources.find(Filename);
	if (it != m_Resources.end())
	{
		return it->second;
	}
	return nullptr;
}

void CTextureManager::Reload(){
	TMapResource::iterator it;
	for (it = m_Resources.begin(); it != m_Resources.end(); ++it)
	{
		it->second->Reload();
	}
}