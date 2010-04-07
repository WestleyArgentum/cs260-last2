/*
this is a message creator designed to receive user input via the wndproc.  If you need things like typing, selecting
stuff, or other functionality, use this message creator in conjunction with something that can send 
VK codes.  If you need to re-map codes against game actions, you will need to change this file.
*/

#pragma once
#include "messagecreator.h"

enum MessageSource{
	KEYDOWN = 0,
	KEYUP,
	KEYPRESS,
	MessageSourceSize
};

class UserInputMessageCreator :
	public MessageCreator
{
public:
	UserInputMessageCreator(void);
	~UserInputMessageCreator(void);
	Message* CreateMessage(int message);
private:
	int messageMap[255];
};
