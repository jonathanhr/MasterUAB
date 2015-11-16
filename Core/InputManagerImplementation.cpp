#include <Windows.h>

#include "InputManagerImplementation.h"

//#include "XML\XMLTreeNode.h"

void CInputManagerImplementation::LoadCommandsFromFile(const std::string& path)
{
	m_Actions.clear();

	//*
	{
		Action action = { "MOVE_LEFT", Action::KEYBOARD };
		action.keyboard.mode = Action::WHILE_PRESSED;
		action.keyboard.key = 'A';
		action.keyboard.needsAlt = false;
		action.keyboard.needsCtrl = false;

		m_Actions.push_back(action);
	}
	{
		Action action = { "MOVE_RIGHT", Action::KEYBOARD };
		action.keyboard.mode = Action::WHILE_PRESSED;
		action.keyboard.key = 'D';
		action.keyboard.needsAlt = false;
		action.keyboard.needsCtrl = false;

		m_Actions.push_back(action);
	}
	{
		Action action = { "MOVE_UP", Action::KEYBOARD };
		action.keyboard.mode = Action::WHILE_PRESSED;
		action.keyboard.key = 'W';
		action.keyboard.needsAlt = false;
		action.keyboard.needsCtrl = false;

		m_Actions.push_back(action);
	}
	{
		Action action = { "MOVE_DOWN", Action::KEYBOARD };
		action.keyboard.mode = Action::WHILE_PRESSED;
		action.keyboard.key = 'S';
		action.keyboard.needsAlt = false;
		action.keyboard.needsCtrl = false;

		m_Actions.push_back(action);
	}
	//*/
	/*CXMLTreeNode l_XML;
	if (l_XML.LoadFile(path.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["input"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Action = l_Input(i);
				Action action = {};
				action.name = l_Action.GetPszProperty("name");
				std::string type = l_Action.GetPszProperty("type", "KEYBOARD");
				action.inputType = ParseInputType(type);
				switch (action.inputType)
				{
				case Action::KEYBOARD:

					action.keyboard.mode = ParseMode(l_Action.GetPszProperty("mode", "ON_PRESS"));
					
					action.keyboard.key = l_Action.GetPszProperty("key", "A")[0];
					action.keyboard.needsAlt = l_Action.GetBoolProperty("needs_alt", false, false);
					action.keyboard.needsCtrl = l_Action.GetBoolProperty("needs_ctrl", false, false);
					break;
				}

				m_Actions.push_back(action);
			}
		}
	}
	*/

	EndFrame();
}

CInputManagerImplementation::Action::Mode CInputManagerImplementation::ParseMode(const std::string& mode)
{
	if (mode == "ON_PRESS")
	{
		return Action::ON_PRESS;
	}
	else if (mode == "ON_RELEASE")
	{
		return Action::ON_RELEASE;
	}
	else if (mode == "WHILE_PRESSED")
	{
		return Action::WHILE_PRESSED;
	}
	else if (mode == "WHILE_RELEASED")
	{
		return Action::WHILE_RELEASED;
	}
	else
	{
		return (Action::Mode) - 1;
	}
}

CInputManagerImplementation::Action::InputType CInputManagerImplementation::ParseInputType(const std::string& inputType)
{
	if (inputType == "KEYBOARD")
	{
		return Action::KEYBOARD;
	}
	else
	{
		return (Action::InputType) - 1;
	}
}

void CInputManagerImplementation::BeginFrame()
{
	m_ActiveActions.clear();

	for (std::vector<Action>::const_iterator it = m_Actions.cbegin(); it != m_Actions.cend(); ++it)
	{
		const Action& action = *it;
		switch (action.inputType)
		{
		case Action::KEYBOARD:
			switch (action.keyboard.mode)
			{
			case Action::ON_PRESS:

				if (m_KeysCurrent[action.keyboard.key] && !m_KeysPrevious[action.keyboard.key])
				{
					if ((!action.keyboard.needsAlt || m_Alt) && (!action.keyboard.needsCtrl || m_Ctrl))
					{
						m_ActiveActions.insert(action.name);
					}
				}
				break;

			case Action::WHILE_PRESSED:

				if (m_KeysCurrent[action.keyboard.key])
				{
					if ((!action.keyboard.needsAlt || m_Alt) && (!action.keyboard.needsCtrl || m_Ctrl))
					{
						m_ActiveActions.insert(action.name);
					}
				}

				break;

			case Action::ON_RELEASE:
			case Action::WHILE_RELEASED:
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

}

void CInputManagerImplementation::EndFrame()
{
	for (int i = 0; i < 256; ++i)
	{
		m_KeysPrevious[i] = m_KeysCurrent[i];
	}

	m_Alt = m_Ctrl = false;
}

bool CInputManagerImplementation::KeyEventReceived(unsigned int wParam, unsigned int lParam)
{
	bool WasDown = ((lParam & (1 << 30)) != 0);
	bool IsDown = ((lParam & (1 << 31)) == 0);
	m_Alt = ((lParam & (1 << 29)) != 0);

	if (WasDown != IsDown)
	{
		if (IsDown)
		{
			switch (wParam)
			{
			case VK_RETURN:
				if (m_Alt)
				{
					m_ActiveActions.insert("TOGLE_FULLCREEN");
					return true;
				}
				return false;
			case VK_ESCAPE:
				PostQuitMessage(0);
				return true;
			case VK_F4:
				if (m_Alt)
				{
					PostQuitMessage(0);
					return true;
				}
				return false;
			}
		}
	}

	m_KeysCurrent[wParam] = IsDown;

	return false;
}
