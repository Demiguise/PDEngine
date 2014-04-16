#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>
#include "IEvent.h"
#include "IEventManager.h"

class CInput
{
public:
	static CInput* GetInstance();
	~CInput();
	void Update(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	CInput();
	static CInput* m_pInstance;
	GameKey* TranslateKeyPress(WPARAM wParam);
};

