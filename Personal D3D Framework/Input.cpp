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
		inputEvent->eData = &wParam;
		eventManager->QueueEvent(inputEvent);
		break;
	case WM_KEYUP:
		inputEvent->eType = "UserKeyRelease";
		inputEvent->eData = &wParam;
		eventManager->QueueEvent(inputEvent);
		break;
	}
}