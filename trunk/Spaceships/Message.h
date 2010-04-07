//a message that can be sent to anything in the game that can receive it.  You will most likely need 
//to make some modifications here for network play to include which host the message came from 
//so that the server knows what to update.  You will also probably need to add an ID to these so that
//it is known what specific game object (if any) this message is meant to apply to.
#pragma once
#include <string>

using namespace std;

enum MessageType{
	SYSTEM,
	GAME_OBJECT_ACTION,
	USER_INPUT
};

enum MessageContent
{
	CREATE_OBJECT = 0,
	DESTROY_OBJECT,
	COLLISION,
	SHOULD_RENDER,
	DO_PHYSICS,
	VISITOR_LEAVE
};

class Message
{
public:
	Message(MessageType type, int message);
	Message(MessageType type, int message, void* extended);
	Message(Message& msg);
	~Message(void);
	MessageType GetMessageType();	
	int GetMessageContent();
	void* GetExtended();
private:
	int _message;
	MessageType _type;
	void* extended;
};
