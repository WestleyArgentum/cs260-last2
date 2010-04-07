#pragma once
#include "messagehandler.h"
#include "dxfactory.h"
#include <list>
#include "DIKeyboardMessageCreator.h"
using namespace std;

class DInputKeyboardhandler :
	public MessageHandler
{
public:
	DInputKeyboardhandler(LPDIRECTINPUTDEVICE8 keyboard);
	virtual ~DInputKeyboardhandler(void);
	virtual MessageCreator* GetMessageCreator(void);

	virtual Message* GetNextMessage(void);
	
	virtual void PassMessage(Message* msg);

	list<Message*> messages;
	bool takeSnapshot;

	void SetHandler(MessageHandler* handler);

protected:
	void SnapShot();
	


private:
	LPDIRECTINPUTDEVICE8 _keyboard;
};
