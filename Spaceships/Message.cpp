#include "Message.h"

Message::Message(MessageType type, int message)
{
	this->_type = type;
	this->_message = message;
	this->extended = NULL;
}

Message::Message(MessageType type, int message, void* extended)
{
	this->_type = type;
	this->_message = message;
	this->extended = extended;
}

Message::Message(Message& msg)
{
	_type = msg._type;
	_message = msg._message;
	extended = msg.extended;
}

Message::~Message(void)
{
}

MessageType Message::GetMessageType(){
	return this->_type;
}

int Message::GetMessageContent(){
	return this->_message;
}

void* Message::GetExtended()
{
	return this->extended;
}

