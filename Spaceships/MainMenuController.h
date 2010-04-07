#pragma once
#include "MenuModel.h"
#include "MenuScreenView.h"


class MainMenuController :
	public Controller
{
public:
	MainMenuController(Model* m, View* v);
	~MainMenuController(void);
	virtual GameState Update();
	virtual void PassMessage(Message* msg);
private:
	void ProcessMessages();
	MenuModel* _model;
	MenuScreenView* _view;
};
