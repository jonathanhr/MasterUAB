#include "TemplatedMapManager.h"
#include "Texture.h"

class CTextureManager : public CTemplatedMapManager<CTexture>
{
public:
	CTextureManager();
	virtual ~CTextureManager();
	CTexture * GetTexture(const std::string &Filename);
	void Reload();
};
