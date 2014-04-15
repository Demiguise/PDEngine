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
	wchar_t newMsg[32];
	switch (msg)
	{
	case WM_KEYDOWN:
		swprintf_s(newMsg, L"Key Pressed: %c\n", (wchar_t)wParam);
		inputEvent->eType = "UserKeyPress";
		inputEvent->eData = &wParam;
		eventManager->QueueEvent(inputEvent);
		break;
	case WM_KEYUP:
		swprintf_s(newMsg, L"Key Released: %c\n", (wchar_t)wParam);
		inputEvent->eType = "UserKeyRelease";
		inputEvent->eData = &wParam;
		eventManager->QueueEvent(inputEvent);
		break;
	}
}