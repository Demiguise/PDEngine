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
	IEvent* inputEvent = new IEvent();
	switch (msg)
	{
	case WM_KEYDOWN:
		inputEvent->eType = "UserKeyPress";
		inputEvent->pDataSize = sizeof(GameKey);
		inputEvent->eData = TranslateKeyPress(wParam);
		eventManager->QueueEvent(inputEvent);
		break;
	case WM_KEYUP:
		inputEvent->eType = "UserKeyRelease";
		inputEvent->pDataSize = sizeof(GameKey);
		inputEvent->eData = TranslateKeyPress(wParam);
		eventManager->QueueEvent(inputEvent);
		break;
	}
}

GameKey* CInput::TranslateKeyPress(WPARAM wParam)
{
	GameKey* keyPress;
	switch(wParam)
	{
	case 65:
		keyPress = new GameKey(GameKey::A);
		break;
	case 68:
		keyPress = new GameKey(GameKey::D);
		break;
	case 83:
		keyPress = new GameKey(GameKey::S);
		break;
	case 87:
		keyPress = new GameKey(GameKey::W);
		break;
	default:
		keyPress = new GameKey(GameKey::A);
	}
	return keyPress;
}