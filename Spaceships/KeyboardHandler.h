#pragma once
#include "MessageHandler.h"
#include "UserInputMessageCreator.h"

class KeyboardHandler :
	public MessageHandler
{
public:
	KeyboardHandler(void);
	~KeyboardHandler(void);
	//gets an object capable of creating a message from some sort of input
	//that this controller will be able to process
	virtual MessageCreator* GetMessageCreator(void);

	virtual Message* GetNextMessage(void);
	////handle the actual input message (whatever it may be)
	virtual void PassMessage(Message* msg);
};
