/*
this is the view that actually handles the game itself.  As long as your Model and Controller are getting the 
correct data to the view, you probably won't need to change this class.
*/

#pragma once
#include "view.h"
#include "GameModel.h"



const LPCSTR backgroundfilename = "background.jpg";

class GameView :
	public View
{
public:
	GameView(Screen* screen, Model* m, LPDIRECT3DDEVICE9 dd3d);
	~GameView(void);
	
	void InitRects();
	
	void SetPlayerPosition(int x, int y, int worldSizeX, int worldSizeY);
	

protected:
	void Draw();
	virtual void PreRender();
	virtual void PostRender();
	void HandleMessage(Message* msg);
private:
	GameModel* _model;
	RECT _bgRect;
	RECT _hudRect;
	LPDIRECT3DTEXTURE9 _backgroundImage;
	bool IsVisible(IRenderable* r);
	
};
