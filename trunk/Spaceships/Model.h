#pragma once
#include "GameState.h"
#include "Controller.h"

class Model
{
public:
	Model();
	~Model(void);
	//when this returns a game state that is different from the current game state, 
	//the game should change states.  Otherwise, it should stay the same.
	GameState SendGameMessage(Message* m);
	void SetCurrentController(Controller* value);

	//purge everything from the model so that it's completely empty.  
	virtual void Eradicate() = 0;
protected:
	//this is the state the game should be in when it is using this model
	GameState _defaultGameState;	
	Controller* controller;	
};

