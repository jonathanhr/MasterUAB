#pragma once

#include <vector>
#include "Utils.h"
#include "Utils\Types.h"

#include "InputManager.h"

class CInputManagerImplementation : public CInputManager
{
public:

	void LoadCommandsFromFile(const std::string& path);

	void BeginFrame();
	void EndFrame();
	bool KeyEventReceived(unsigned int wParam, unsigned int lParam);

private:
	struct Action
	{
		std::string name;

		enum Mode {
			ON_PRESS,
			ON_RELEASE,
			WHILE_PRESSED,
			WHILE_RELEASED
		};

		enum InputType {
			KEYBOARD,
		} inputType;
		union {
			struct Keyboard {
				Mode mode;
				uint8_t key;
				bool needsAlt;
				bool needsCtrl;
			} keyboard;
		};
	};

	Action::Mode ParseMode(const std::string& mode);
	Action::InputType ParseInputType(const std::string& inputType);

	bool m_KeysCurrent[256], m_KeysPrevious[256];
	bool m_Alt, m_Ctrl;

	std::vector<Action> m_Actions;
};

