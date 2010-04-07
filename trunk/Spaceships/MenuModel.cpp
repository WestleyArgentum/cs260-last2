#include "MenuModel.h"

MenuModel::MenuModel(void)
{
	//this is the Main Menu
	this->_defaultGameState = GameState::MainMenu;
	this->_selectedMenuItem = 0;

	//we need a text object for the main menu.
}

MenuModel::~MenuModel(void)
{
	this->Eradicate();
}

void MenuModel::Eradicate(){

}
GameState MenuModel::SendGameMessage(Message* m){


	return this->_defaultGameState;

}

int MenuModel::GetSelectedMenuItem(){
	return this->_selectedMenuItem;
}

void MenuModel::IncrementMenuItemDown(){
	if(this->_selectedMenuItem == 2)
		this->_selectedMenuItem = 0;
	else
		this->_selectedMenuItem++;
}

void MenuModel::IncrementMenuItemUp(){
	if(this->_selectedMenuItem == 0)
		this->_selectedMenuItem = 2;
	else
		this->_selectedMenuItem--;
}