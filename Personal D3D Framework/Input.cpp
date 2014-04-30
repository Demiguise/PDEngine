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
	case 65:
		return GameKey::A;
	case 68:
		return GameKey::D;
	case 83:
		return GameKey::S;
	case 87:
		return GameKey::W;
	default:
		return GameKey::A;
	}
}