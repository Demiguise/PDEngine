#include "Input.h"


CInput* CInput::m_pInstance = 0;

CInput* CInput::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CInput();
	}
	return m_pInstance;
}

CInput::CInput()
{
	GameLog::GetInstance()->Log(DebugChannel::Input, DebugLevel::None, "[InputManager] Testing Input Channel! %i", 3);
}


CInput::~CInput()
{
}

void CInput::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	IEventManager* eventManager = IEventManager::GetInstance();
	InputEvent* inputEvent = new InputEvent();
	switch (msg)
	{
	case WM_MOUSEMOVE:
		inputEvent->eType = "UserMouseMove";
		inputEvent->mouseMovement = EnVector2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_KEYDOWN:
		inputEvent->eType = "UserKeyPress";
		inputEvent->keyEvent = TranslateKeyPress(wParam);
		break;
	case WM_KEYUP:
		inputEvent->eType = "UserKeyRelease";
		inputEvent->keyEvent = TranslateKeyPress(wParam);
		break;
	}
	eventManager->QueueEvent(inputEvent);
}

GameKey CInput::TranslateKeyPress(WPARAM wParam)
{
	switch(wParam)
	{
	case 0x41:
		return GameKey::A;
	case 0x44:
		return GameKey::D;
	case 0x45:
		return GameKey::E;
	case 0x51:
		return GameKey::Q;
	case 0x53:
		return GameKey::S;
	case 0x57:
		return GameKey::W;
	}
}