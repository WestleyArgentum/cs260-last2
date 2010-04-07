#include "UserInputMessageCreator.h"


UserInputMessageCreator::UserInputMessageCreator(void)
{
	for(int i = 0; i < 255; i++)
		this->messageMap[i] = 0;	

	messageMap[VK_ESCAPE] = GameAction::QUIT;
	messageMap[VK_UP] = GameAction::MOVE_UP;
	//messageMap[VK_UP] = GameAction::STOP;

	messageMap[VK_DOWN] = GameAction::MOVE_DOWN;
	messageMap[VK_RETURN] = GameAction::SELECT;
	
}

UserInputMessageCreator::~UserInputMessageCreator(void)
{
}

Message* UserInputMessageCreator::CreateMessage(int message){
	Message* msg = NULL;

	msg = new Message(MessageType::USER_INPUT, messageMap[message]);
	return msg;
}