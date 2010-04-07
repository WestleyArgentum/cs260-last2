/*
this is the view for the main menu.  it renders the text and shows the  user's selections  You probably won't
need to change this class
*/

#pragma once
#include "MenuModel.h"
#include "View.h"
#include "Screen.h"
#include "d3dx9.h"
#include "d3d9.h"
#include <string>
using namespace std;

const int MenuChoicesCount = 2;
const LPCSTR _menuLabelText = "SPACESHIPS!";
const int _menuLabelTextLen = strlen(_menuLabelText);

const LPCSTR _menuNewGameOption = "New Game";
const int _menuNewGameOptionLen = strlen(_menuNewGameOption);

const LPCSTR _menuNetworkGameOption = "Play Network Game";
const int _menuNetworkGameOptionLen = strlen(_menuNetworkGameOption);

const LPCSTR _menuQuitOption = "Quit";
const int _menuQuitOptionLen = strlen(_menuQuitOption);

const D3DCOLOR _selTextColor = D3DCOLOR_ARGB(255,255,255,255);;
const D3DCOLOR _normalTextColor = D3DCOLOR_ARGB(150,255,255,255);;

class MenuScreenView :
	public View
{
public:
	MenuScreenView(Screen* screen, Model* m, LPDIRECT3DDEVICE9 dd3d);
	~MenuScreenView(void);	
	
	void InitRects();
protected:
	void PreRender();
	void PostRender();
	void Draw();
	void HandleMessage(Message* msg);
private:
	LPD3DXFONT _mainMenuFont;
	LPD3DXFONT _subMenuFont;
	MenuModel* _model;	
	RECT _titleRect;
	RECT _menuRect[3];	


};
