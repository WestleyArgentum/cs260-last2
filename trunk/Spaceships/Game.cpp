#include "precompiled.h"
#include "Game.h"
using namespace std;

extern MessageHandler* G_globalMessageHandler;

Game::Game(Screen* screen, DXFactory* factory)
{
	//default to main menu
	internalState = GameState::MainMenu;
	_screen = screen;	
	for(int i = 0; i < GameStateCount; i++){
		this->_controllers[i] = NULL;
		this->_handlers[i] = NULL;
	}

	this->_factory = factory;

}

Game::~Game(void)
{	
}

void Game::AddController(Controller* c, GameState g){
	_controllers[g] = c;

}

bool Game::changeState(GameState newState){
	internalState = newState;	
	return true;
}

GameState Game::getState(){ 
	return internalState;
}

void Game::AddView(View* v){
	this->_currentView = v;
}

int Game::InitGameLoop(){

	DWORD lastTickCount;
	DWORD currentTickCount;

	lastTickCount = GetTickCount();
	list<Message*> temp;

	while(1){
		//TODO:  you'll need to route messages that the user created to the network somehow
		Message* msg = _handlers[internalState]->GetNextMessage();

		//TODO:  you'll need to get the messages from the network somehow so that they can be 
		//passed to the controller for handling.

		//get every message and pass it into our temporary list for processing.
		//this will free up our list to receive more input if it needs to.  not 
		//terribly important for directinput but the wndproc is event driven so this
		//is critical
		while(msg  != NULL)
		{
			
			if(msg->GetMessageType() == MessageType::USER_INPUT){
				if(msg->GetMessageContent() == (GameAction)GameAction::QUIT)
				{
					delete msg;
					return 0;
				}
				else
				{
					_controllers[internalState]->PassMessage(msg);
				}
			}
			msg = _handlers[internalState]->GetNextMessage();
		}

		//tell the controller to process all the messages you just aggregated for it
		// this->_controllers[internalState]->HandleMessages(&temp);

		//initialize drawing the screen
		_screen->GetBackBuffer();

		//tell the game to update its state and draw
		//imagine if this were a switch statement on internalState.  If we added a new state,
		//would this code have to change (hint: no)
		GameState gs = this->_controllers[internalState]->Update();

		//tell the screen to flip the back buffer
		_screen->UpdateScreen();
		
		//change state if needed
		if(internalState != gs)
		{
			internalState = gs;
		}

		if(gs == GameState::Quit)
			return 0;
		
		//lock timing 
		currentTickCount = GetTickCount();

		int diff = currentTickCount - lastTickCount;

		int singelframetime = 1000 / FRAMERATE;

		if(diff < singelframetime){		
			Sleep(singelframetime - diff);
		}

		lastTickCount = currentTickCount;

	}
}

//no way to get the handler except with a static call.  This is like a singleton.  
//probably the global used to store the handler could be removed
MessageHandler* Game::GetWindProcHandler(void){
	if(G_globalMessageHandler == NULL)
		G_globalMessageHandler = new KeyboardHandler();

	return G_globalMessageHandler;	
}

//add a handler for a particular game state.  If you add a new game state, 
//you'll need to ensure that a handler is registered for it.  You may need to 
//mess with these so that handlers know about the network somehow
void Game::RegisterMessageHandler(GameState g, MessageHandler* handler){
	
	this->_handlers[g] = handler;
}



