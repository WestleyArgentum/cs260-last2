#pragma once
#include "IVisitor.h"
#include "GameObject.h"

class GameObjectVisitor : IVisitor
{
public:
	GameObjectVisitor(void);
	~GameObjectVisitor(void);
	void Visit(GameObject* object);
	void Leave();
	void VisitorAccepted(GameObject* objectToVisit, Controller* controller);
	void PassMessage(Message* msg);
	void ProcessMessages();

protected:
	virtual void OnAccept() = 0;
	virtual void OnLeave() = 0;
	virtual void OnVisit(GameObject* object) = 0;
	virtual void ProcessMessage(Message* msg) = 0;
	GameObject* objectVisiting;
	list<Message*> messages;
	Controller* owningController;
};

