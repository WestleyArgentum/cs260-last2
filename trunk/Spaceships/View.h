/*
the base view class.  This is what actually is responsible for drawing to the screen.  I really need to 
do some refactoring to extract the actual rendering responsibility away from the view.  the view is 
only a place to gather and present the objects and actually doing the rendering is an additional responsibility.
if I have time I'll fix this but if you're reading this than I probably did not.  You probably won't need
to change this class
*/

#pragma once
#include "Model.h"

class IRenderable;

class View
{
public:
	View(Screen* screen, Model* m, LPDIRECT3DDEVICE9 d3dd);
	void DrawScreen();
	virtual ~View(void);
	void SetSprite(LPD3DXSPRITE s);
	void LogError(int err, string text);
	LPDIRECT3DDEVICE9 GetDevice();
	LPD3DXSPRITE GetSprite();
	void PassMessage(Message* msg);
	void ProcessMessages();
	RECT GetViewRect();
	LPD3DXFONT GetErrorTextFont();
	// currentPosition is an output parameter
	void* LockBuffer(int size);
	void UnlockBuffer(int bufferSize);
	void AddPointToRender(Vertex v);
	void SetBackground(LPDIRECT3DTEXTURE9 bgtext);

protected:
	virtual void Draw() = 0;	
	virtual void PreRender() = 0;
	virtual void PostRender() = 0;
	virtual void HandleMessage(Message* msg) = 0;
	Screen* screen;
	Model* _model;
	LPDIRECT3DDEVICE9 _dd3dDevice;
	LPD3DXSPRITE _sprite;
	LPD3DXFONT _errorFont;
	RECT screenRect;
	LPDIRECT3DTEXTURE9 background;

	int pointBufferOffset;
	void* pointBufferPointer;

	list<IRenderable*> renderables;
	list<Vertex> points;
	list<Message*> messages;

	//current size of line buffer- the max number of lines that this thing can hold
	int _lineBufferSize;

	//current size of point buffer- max number of points that this thing can hold
	int _pointBufferSize;

	RECT errRect1;
	RECT errRect2;
	RECT errRect3;
	LPDIRECT3DVERTEXBUFFER9  pointBuffer;
	LPDIRECT3DVERTEXBUFFER9  lineBuffer;

};
