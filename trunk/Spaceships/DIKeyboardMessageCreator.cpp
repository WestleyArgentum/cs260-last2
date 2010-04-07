#include "DIKeyboardMessageCreator.h"

int global_firecount = 0;

DIKeyboardMessageCreator::DIKeyboardMessageCreator(void)
{
	SecureZeroMemory(&this->messageMap, sizeof(messageMap));

	//initialize those elements of this thing that are going to be actions
	this->messageMap[DIK_ESCAPE] = GameAction::QUIT;
	this->messageMap[DIK_SPACE] = GameAction::FIRE_PRIMARY;
	this->messageMap[DIK_RETURN] = GameAction::SELECT;
	this->messageMap[DIK_W] = GameAction::MOVE_UP;
	this->messageMap[DIK_A] = GameAction::MOVE_LEFT;
	this->messageMap[DIK_D] = GameAction::MOVE_RIGHT;
	this->messageMap[DIK_F] = GameAction::FIRE_SECONDARY;
}

DIKeyboardMessageCreator::~DIKeyboardMessageCreator(void)
{
	
}

Message* DIKeyboardMessageCreator::CreateMessage(int message){
	//DO_NOTHING is the default value in our map, this is necessary so that
	//zeroing the array won't cause any keystroke to be MOVE_UP (found that out the hard way)
	/*if(messageMap[message] == GameAction::FIRE_PRIMARY){
		GlobalDebug::GetInstance()->DbgOutInt("\r\nfire primary", global_firecount++);
	}*/

	

	if(messageMap[message] != DO_NOTHING)
		return new Message(MessageType::USER_INPUT, messageMap[message]);
	else
		return NULL;
}