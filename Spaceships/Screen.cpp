#include "Screen.h"

Screen::Screen(HWND window, RECT screensize, LPDIRECT3DDEVICE9 device)
{
	this->_screensize = screensize;
	this->_window = window;
	this->_device = device;
	D3DXCreateSprite(this->_device, &this->_spriteInterface);
	sceneCounter = 0;
}

Screen::~Screen(void)
{
	this->_spriteInterface->Release();
	GlobalDebug::GetInstance()->DbgOut("Screen::~Screen");
}

void Screen::SetWindowHandle(HWND window)
{
	this->_window = window;
}

HWND Screen::GetWindowHandle(HWND window){
	return this->_window;
}

void Screen::GetBackBuffer()
{
	HRESULT result;	
	result = _device->Clear(0, NULL, D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	if(result != D3D_OK)
	{
		GlobalDebug::GetInstance()->DbgOut("Screen::GetBackBuffer Error clearing d3ddevice");
	}
	result = _device->BeginScene();
	sceneCounter += 1;
	if(result != D3D_OK)
	{
		GlobalDebug::GetInstance()->DbgOut("Screen::GetBackBuffer Error on beginscene");
	}
}
void Screen::Flush()
{
	_device->EndScene();
	sceneCounter -= 1;
	HRESULT result = _device->Present(NULL, NULL, NULL, NULL);
	if(result != D3D_OK || result != S_OK)
	{
		if(result == D3DERR_DEVICEREMOVED)
			GlobalDebug::GetInstance()->DbgOut("Screen::Flush Error EndScene: device removed");

		else if(result == D3DERR_INVALIDCALL)
			GlobalDebug::GetInstance()->DbgOut("Screen::Flush Error EndScene: invalid call");
		else
			GlobalDebug::GetInstance()->DbgOut("Screen::Flush Error EndScene: unknown error");
		
	}
	sceneCounter += 1;
	_device->BeginScene();
}

void Screen::UpdateScreen()
{
	assert(sceneCounter > 0);
	GlobalDebug::AssertAndLogResult(_device->EndScene(), "Screen::UPDATESCREEN");	
	sceneCounter--;
	assert(sceneCounter == 0);
	GlobalDebug::AssertAndLogResult(_device->Present(NULL, NULL, NULL, NULL), "Screen::UPDATESCREEN");	return;
}

RECT Screen::GetScreenSize()
{
	return _screensize;
}