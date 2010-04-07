#pragma once
#include "Controller.h"

// TODO: get time machine, find person who implemented this build crap, and destroy them!
class Controller;

class IVisitor
{
public:
	virtual void Visit(GameObject* object) = 0;
	virtual void Leave() = 0;
	virtual void VisitorAccepted(GameObject* objectToVisit, Controller* controller) = 0;
	virtual void PassMessage(Message* msg) = 0;
	virtual void ProcessMessages() = 0;
};