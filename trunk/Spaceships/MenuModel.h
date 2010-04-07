/*
the model for the Main menu.  You'll possibly need to change this depending on what you need to add for
UI for the user to play online.
*/

#pragma once
#include "model.h"
#include "GameObject.h"

class MenuModel :
	public Model
{
public:
	MenuModel(void);
	~MenuModel(void);
	GameObject* GetMenuItems();
	const int MenuItemCount();
	int GetSelectedMenuItem();
	GameState SendGameMessage(Message* m);
	void IncrementMenuItemUp();
	void IncrementMenuItemDown();
	void Eradicate();
	
private:
	int _menuItemCount;
	int _selectedMenuItem;


};
