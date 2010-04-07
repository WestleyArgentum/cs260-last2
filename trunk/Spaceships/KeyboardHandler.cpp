#include "KeyboardHandler.h"


extern CRITICAL_SECTION wndprocsync;

KeyboardHandler::KeyboardHandler(void)
{
	this->_messageCreator = NULL;
	
}

KeyboardHandler::~KeyboardHandler(void)
{
}

MessageCreator* KeyboardHandler::GetMessageCreator(void){
	if(this->_messageCreator == NULL)
		this->_messageCreator = new UserInputMessageCreator();

	return this->_messageCreator;

}

void KeyboardHandler::PassMessage(Message* msg){
	EnterCriticalSection(&wndprocsync);
	this->messages.push_back(msg);

	// prevent a crapload of messages from building up if we don't 
	// end up checking this thing.  Bonus:  Find a way to turn this off
	if(messages.size() > 50)
	{
		Message* msg = messages.front();
		messages.pop_front();
		delete msg;
	}
	LeaveCriticalSection(&wndprocsync);
}

Message* KeyboardHandler::GetNextMessage(void){
	
	EnterCriticalSection(&wndprocsync);	
	if(messages.empty()){
		LeaveCriticalSection(&wndprocsync);	
		return NULL;
	}

	Message* temp = this->messages.front();
	this->messages.pop_front();
	
	LeaveCriticalSection(&wndprocsync);
	return temp;
}