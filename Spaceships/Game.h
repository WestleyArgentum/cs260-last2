#pragma once
#include "MessageHandler.h"
#include "KeyboardHandler.h"
#include "GameAction.h"
#include "Screen.h"
#include "View.h"
#include "Controller.h"
#include "DInputKeyboardhandler.h"

/*
this does the majority of the work driving the game.  It collects messages from the various message handlers
that are mapped to whatever game state you're in and passes them to the correct controller.  It also 
handles timing.  You will need to change how messages are sent and to where so this is the place to do that.
*/

using namespace std;

//frames per second
const int FRAMERATE = 30;

class Game
{
public:
	Game(Screen* screen, DXFactory* factory);	
	~Game(void);
	bool changeState(GameState newState);
	GameState getState();
	int InitGameLoop();
	void AddView(View* v);
	static MessageHandler* GetWindProcHandler(void);
	void AddController(Controller* c, GameState g);
	void RegisterMessageHandler(GameState g, MessageHandler* handler);
	static void PassToCurrentController(Message* msg);
	
protected:
	GameState internalState;	

private:
	Screen* _screen;
	View* _currentView;
	Controller* _currentController;
	Controller* _controllers[GameStateCount];	
	MessageHandler* _handlers[GameStateCount];
	Controller* _MainMenuController;
	 DXFactory* _factory;
};



