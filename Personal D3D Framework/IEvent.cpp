#include "IEvent.h"

IEvent::IEvent()
{
	eData = NULL;
	pDataSize = 0;
	eType = "UnInitialised";
}


IEvent::~IEvent()
{
	if (eData != NULL)
	{
		OutputDebugString(L"Event data has not been deleted. Attempting Cleanup.\n");
		//I don't even know if this is a good idea or not, choosing to believe it isn't but at the moment it 'Works'.
		//Honestly, I think it's deleting a hell of a lot more data I think it should. It could be all padding but I'm quite unsure.
		delete eData; 
	}
}