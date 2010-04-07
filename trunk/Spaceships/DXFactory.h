/*
this file controls all the init and teardown of all things directx.  you probably will not need to touch this file
at all.
*/
#pragma once

#include "d3d9.h"
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

const int DIKEYBOARDBUFFERSIZE = 255;


class DXFactory
{
public:
	DXFactory(HINSTANCE instance, HWND window);
	~DXFactory(void);
	void Init();
	LPDIRECT3DDEVICE9 GetGraphicsDevice();
	LPD3DXSPRITE GetSprite();
	LPDIRECTINPUTDEVICE8 GetKeyboard();
	DIPROPDWORD GetKeyboardProperties();

protected:
	
LPDIRECT3D9 d3dMain;
LPDIRECTINPUT8 dInput;

LPDIRECT3DDEVICE9 d3dDevice;
LPDIRECTINPUTDEVICE8 keyboard;

LPD3DXSPRITE sprite;
DIPROPDWORD diKbproperties;

HINSTANCE appInstance;
HWND appWindow;
};
