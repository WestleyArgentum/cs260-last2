#include "View.h"

View::View(Screen* screen, Model* m, LPDIRECT3DDEVICE9 d3dd)
{
	background = NULL;
	this->screen = screen;
	this->_model = m;
	this->_dd3dDevice = d3dd;

	this->_lineBufferSize = 1000;
	this->_pointBufferSize = 10000;

	pointBufferOffset = 0;
	this->pointBufferPointer = NULL;

	d3dd->CreateVertexBuffer(_pointBufferSize*sizeof(Vertex), 0, D3DFVF_SPACESHIPSVERTEX, D3DPOOL_DEFAULT, &pointBuffer, NULL);
	d3dd->CreateVertexBuffer(_lineBufferSize*sizeof(Vertex), 0, D3DFVF_SPACESHIPSVERTEX, D3DPOOL_DEFAULT, &lineBuffer, NULL);
	
	//TODO: push this into font factory
	D3DXCreateFont(
		this->_dd3dDevice, 10, 0, FW_NORMAL, 1, 
		false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, 
		"Arial", &this->_errorFont);


	//init error rects for drawing errors
	errRect1.top = 0;
	errRect1.left = 0;
	errRect1.right = 500;
	errRect1.bottom = 15;

	errRect2.top = 15;
	errRect2.left = 0;
	errRect2.right = 500;
	errRect2.bottom = 30;

	errRect3.top = 30;
	errRect3.left = 0;
	errRect3.right = 500;
	errRect3.bottom = 45;


}

View::~View(void)
{
	while(this->messages.size() > 0)
	{
		Message* m = messages.front();
		messages.pop_front();
		delete m;
	}

	while(this->renderables.size() > 0)
	{
		IRenderable* r = renderables.front();
		renderables.pop_front();
		delete r;
	}

	// reduce memory leaks
	pointBuffer->Release();
	lineBuffer->Release();
	_errorFont->Release();
}

void View::SetSprite(LPD3DXSPRITE s){

	this->_sprite = s;
}

void View::LogError(int err, string text){

	RECT r;
	if(err == 1)
		r = errRect1;
	else if(err == 2)
		r = errRect2;
	else
		r = errRect3;

	LPCSTR str = text.c_str();

	HRESULT res = _errorFont->DrawTextA(this->GetSprite(), 
		str, strlen(str), &r, DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

	if(FAILED(res))
		throw;

}

LPDIRECT3DDEVICE9 View::GetDevice()
{
	return _dd3dDevice;
	
}
LPD3DXSPRITE View::GetSprite()
{
	return _sprite;
}

void View::PassMessage(Message* msg)
{
	messages.push_back(msg);
}

void View::DrawScreen()
{
	list<Message*>::iterator msgIterator;
	for(msgIterator = messages.begin(); msgIterator != messages.end(); ++ msgIterator)
	{
		HandleMessage((Message*)*msgIterator);
	}

	messages.clear();

	// now draw the screen
	PreRender();
	Draw();
	PostRender();
}

RECT View::GetViewRect()
{
	return screenRect;
}

LPD3DXFONT View::GetErrorTextFont()
{
	return _errorFont;
}

void* View::LockBuffer(int size)
{
	pointBuffer->Lock(this->pointBufferOffset, size, (void**)&pointBufferPointer, 0);
	return this->pointBufferPointer;
}

void View::UnlockBuffer(int bufferSize)
{
	pointBuffer->Unlock();
	pointBufferOffset += bufferSize;
}

void View::AddPointToRender(Vertex v)
{
	points.push_back(v);
}

void View::SetBackground(LPDIRECT3DTEXTURE9 bgtext)
{
	background = bgtext;
}