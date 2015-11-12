#include "InputManager.h"

static CInputManager* s_CurrentInputManager = nullptr;

CInputManager* CInputManager::GetInputManager()
{
	return s_CurrentInputManager;
}

void CInputManager::SetCurrentInputManager(CInputManager* _InputManager)
{
	s_CurrentInputManager = _InputManager;
}
