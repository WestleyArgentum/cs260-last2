#include "MenuScreenView.h"

MenuScreenView::MenuScreenView(Screen* screen, Model* m, LPDIRECT3DDEVICE9 dd3d): View(screen, m, dd3d)
{
	this->_model = (MenuModel*)m;
	this->screen = screen;

	D3DXCreateFont(
		this->_dd3dDevice, 60, 0, FW_NORMAL, 1, 
		false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, 
		"Arial", &this->_mainMenuFont);
	
	D3DXCreateFont(
		this->_dd3dDevice, 40, 0, FW_NORMAL, 1, 
		false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, 
		"Arial", &this->_subMenuFont);

	InitRects();
}

void MenuScreenView::InitRects(){

	SetRect(&this->_titleRect, 200, 100, 600, 200);
	SetRect(&this->_menuRect[0], 200,200,600,300);
	SetRect(&this->_menuRect[1], 200,300,600,400);
	SetRect(&this->_menuRect[2], 200,400,600,500);

}

MenuScreenView::~MenuScreenView(void)
{
	this->_mainMenuFont->Release();
	this->_subMenuFont->Release();
}

void MenuScreenView::Draw(){



HRESULT res; 

	
	if(background != NULL)
	{
		GlobalDebug::GetInstance()->AssertAndLogResult(_sprite->Draw(this->background, NULL, NULL, NULL, D3DCOLOR_XRGB(255,255,255)), "MenuScreenView::Draw background");
		_sprite->End();
		_sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}


	
	// draw this thing
	// draw the title
	res = _mainMenuFont->DrawTextA(NULL, _menuLabelText, strlen(_menuLabelText), &this->_titleRect, DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	// figure out selected menu item
	int item = this->_model->GetSelectedMenuItem();

	res = _subMenuFont->DrawTextA(NULL, _menuNewGameOption, _menuNewGameOptionLen, &this->_menuRect[0] , DT_LEFT | DT_VCENTER, item == 0 ? _selTextColor : _normalTextColor);	
	res = _subMenuFont->DrawTextA(NULL, _menuNetworkGameOption, _menuNetworkGameOptionLen, &this->_menuRect[1], DT_LEFT | DT_VCENTER, item == 1 ? _selTextColor : _normalTextColor);
	res = _subMenuFont->DrawTextA(NULL, _menuQuitOption, _menuQuitOptionLen, &this->_menuRect[2], DT_LEFT | DT_VCENTER, item == 2 ? _selTextColor : _normalTextColor);
}

void MenuScreenView::HandleMessage(Message* msg)
{
	delete msg;
}

void MenuScreenView::PreRender()
{
		GlobalDebug::GetInstance()->AssertAndLogResult(_sprite->Begin(D3DXSPRITE_ALPHABLEND), "View::PreRender Sprite::Begin");

}

void MenuScreenView::PostRender()
{
		GlobalDebug::GetInstance()->AssertAndLogResult(_sprite->End(), "View::PostRender Sprite::End");
}