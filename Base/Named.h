#include <string>
#include <stdlib.h>

#include "XML\XMLTreeNode.h"

class CNamed
{
	protected:
		std::string m_Name;

	public:
		CNamed(const CXMLTreeNode &TreeNode);
		CNamed(const std::string &Name);
	
		virtual void SetName(const std::string &Name);
		virtual const std::string &GetName();
};
