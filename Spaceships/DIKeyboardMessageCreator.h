/*
this creates messages from DirectInput.  It is designed to get keyboard input 
in real-time and generate a message for any key that is down that happens to be mapped.  If you add any useful
game functionality or implement any new features or wish to change the keyboard map for any reason, this 
would be the place to do it.
*/

#pragma once
#include "messagecreator.h"
#include "windows.h"
#include <dinput.h>

class DIKeyboardMessageCreator :
	public MessageCreator
{
public:
	DIKeyboardMessageCreator(void);
	~DIKeyboardMessageCreator(void);
	Message* CreateMessage(int message);
	int messageMap[255];
};
