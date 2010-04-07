#include "MainMenuController.h"

MainMenuController::MainMenuController(Model* m, View* v):Controller(m, v)
{
	this->_model = (MenuModel*)m;
	this->_view = (MenuScreenView*)v;
	this->newGameState = GameState::MainMenu;
}

MainMenuController::~MainMenuController(void)
{
	
}

GameState MainMenuController::Update(){

	ProcessMessages();
	//determine if we have a state change for some reason
	//otherwise, time to render the view
	this->_view->DrawScreen();

	//return MainMenu;
	return this->newGameState;


}


// TODO: move to base class
void MainMenuController::PassMessage(Message* msg)
{
	this->messages.push_back(msg);
}

// TODO: move to base class
void MainMenuController::ProcessMessages()
{
	//handle each message by updating the model
	this->newGameState = GameState::MainMenu;

	while(!messages.empty()){
		Message* temp = messages.front();
		messages.pop_front();
		if(temp->GetMessageType() == MessageType::USER_INPUT){
			//we care about user input here, nothing else really

			switch(temp->GetMessageContent())
			{

				case GameAction::MOVE_UP:
					_model->IncrementMenuItemUp();
					break;

				case GameAction::MOVE_DOWN:
					_model->IncrementMenuItemDown();
					break;

				case GameAction::SELECT:
					//user has selected something, probably a state change
					int selection = this->_model->GetSelectedMenuItem();
					if(selection == 2)
						this->newGameState = GameState::Quit;
					if(selection == 0)
						this->newGameState = GameState::Playing;
					break;
			}
			//clean up the message now that it's been processed
			delete temp;
		}
	}
	assert(messages.size() == 0);
}