#pragma once

#include <string>
#include <unordered_set>

class CInputManager
{
public:

	bool IsActionActive(const std::string& action) { return m_ActiveActions.count(action) > 0; }


	static CInputManager* GetInputManager();
	static void SetCurrentInputManager(CInputManager* _InputManager);

protected:

	std::unordered_set<std::string> m_ActiveActions;
};

