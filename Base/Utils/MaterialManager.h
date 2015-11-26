#ifndef _MATERIAL_MANAGER
#define _MATERIAL_MANAGER

#pragma once

#include "TemplatedMapManager.h"
#include "Material.h"

#include <string>

class CMaterialManager : public CTemplatedMapManager<CMaterial>
{
private:
	std::string m_Filename;
public:
	CMaterialManager();
	virtual ~CMaterialManager();
	void Load(const std::string &Filename);
	void Reload();
};

#endif