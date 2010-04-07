#pragma once
#include "controller.h"
#include "gameaction.h"
#include "gamestate.h"

#include "GameModel.h"
 
class PhysicsController :
	public Controller
{
public:
	PhysicsController(Model* m, View* v);
	~PhysicsController(void);
	virtual void HandleMessages(list<Message*>* l);
	virtual GameState Update();
	virtual void PassMessage(Message* msg);
};
