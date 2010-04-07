#pragma once
#include "ObjectReaper.h"

// this is why they call it "dll hell"
class Model;
class View;

class Controller
{
public:
	Controller(Model* m, View* v);
	virtual ~Controller(void);
	virtual GameState Update() = 0;
	virtual void PassMessage(Message* msg) = 0;
protected:
	GameState newGameState;
	virtual void ProcessMessages() = 0;
	list<Message*> messages;
	ObjectReaper theReaper;
};
