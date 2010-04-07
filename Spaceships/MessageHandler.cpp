#include "MessageHandler.h"

MessageHandler::MessageHandler(void)
{
	InitializeCriticalSection(&this->listLock);
}

MessageHandler::~MessageHandler(void)
{
	if(this->_messageCreator != NULL)
		delete _messageCreator;

	while(messages.size() > 0)
	{
		Message* m = messages.front();
		messages.pop_front();
		delete m;
	}

	DeleteCriticalSection(&listLock);
}

