#pragma once


#include "MessageCreator.h"
using namespace std;
class MessageHandler
{
public:
	MessageHandler(void);
	virtual ~MessageHandler(void);

	//gets an object capable of creating a message from some sort of input
	//that this controller will be able to process
	virtual MessageCreator* GetMessageCreator(void) = 0;

	//handle the actual input message (whatever it may be)
	virtual void PassMessage(Message* msg) = 0;
	virtual Message* GetNextMessage(void) = 0;
	//virtual list<Message*>* GetCurrentQueuedMessages() = 0;

protected:
	MessageType _handlesMessage;
	MessageCreator* _messageCreator;
	list<Message*> messages;
	CRITICAL_SECTION listLock;

};

