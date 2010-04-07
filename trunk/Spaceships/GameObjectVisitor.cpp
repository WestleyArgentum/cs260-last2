#include "precompiled.h"
#include "GameObjectVisitor.h"

GameObjectVisitor::GameObjectVisitor(void)
{
}

GameObjectVisitor::~GameObjectVisitor(void)
{
}

void GameObjectVisitor::VisitorAccepted(GameObject* objectToVisit, Controller* controller)
{
	objectVisiting = objectToVisit;
	owningController = controller;
	OnAccept();
}

void GameObjectVisitor::Leave()
{
	OnLeave();
	this->objectVisiting->PassMessageDirect(
		new Message(MessageType::GAME_OBJECT_ACTION, MessageContent::VISITOR_LEAVE, (void*)this));
}

void GameObjectVisitor::Visit(GameObject* object)
{
	OnVisit(object);
}

void GameObjectVisitor::PassMessage(Message* msg)
{
	messages.push_back(msg);
}

void GameObjectVisitor::ProcessMessages()
{
	list<Message*>::iterator msgIter;	
	while(messages.size() > 0)
	{
		Message* message = messages.front();
		messages.pop_front();
		ProcessMessage(message);
	}
}