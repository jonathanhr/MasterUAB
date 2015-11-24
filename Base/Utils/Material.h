#include "Named.h"
#include "Texture\Texture.h"

#include <vector>

class CMaterial : public CNamed
{
private:
	std::vector<CTexture *> m_Textures;
	//CEffectTechnique *m_EffectTechnique;
	void Destroy();
public:
	CMaterial(CXMLTreeNode &TreeNode);
	virtual ~CMaterial();
	virtual void Apply();
	//UAB_GET_PROPERTY(CEffectTechnique, EffectTechnique);
};