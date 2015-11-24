#include "MaterialManager.h"	

CMaterialManager::CMaterialManager(){};
CMaterialManager::~CMaterialManager(){
	Destroy();
};

void CMaterialManager::Load(const std::string &Filename){
	m_Filename = Filename;
}

void CMaterialManager::Reload(){
	
}