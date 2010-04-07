#include "DXFactory.h"


DXFactory::DXFactory(HINSTANCE instance, HWND window)
{
	this->appInstance = instance;
	this->appWindow = window;
	this->d3dDevice = NULL;
	this->d3dMain = NULL;
	this->dInput = NULL;
	this->keyboard = NULL;
	this->sprite = NULL;
}

DXFactory::~DXFactory(void)
{
	this->sprite->Release();	
	this->keyboard->Release();
	this->dInput->Release();
	this->d3dDevice->Release();	
	this->d3dMain->Release();		
	
}

void DXFactory::Init(void){
	HRESULT ddrval;

	//initialize directx stuff

	if(d3dMain == NULL){
		d3dMain = Direct3DCreate9(D3D_SDK_VERSION);
	}

	if(d3dMain == NULL)
		throw;

	D3DPRESENT_PARAMETERS d3dpp = {0};
	SecureZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.hDeviceWindow = appWindow;

	if(d3dDevice == NULL){
		ddrval = d3dMain->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			appWindow,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&d3dDevice);
	}

	d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	if(FAILED(ddrval))
		throw;

	if(sprite == NULL)
		ddrval = D3DXCreateSprite(d3dDevice, &sprite);
	if(FAILED(ddrval))
		throw;

	if(dInput == NULL)
		ddrval = DirectInput8Create(this->appInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, NULL);

	if(FAILED(ddrval))
		throw;

	LPDIRECTINPUTDEVICE8 temp = NULL;
	dInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	//keyboard = temp;
	keyboard->SetDataFormat(&c_dfDIKeyboard);

	keyboard->SetCooperativeLevel(this->appWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);



	diKbproperties.diph.dwSize = sizeof( DIPROPDWORD );
	diKbproperties.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	diKbproperties.diph.dwObj = 0;
	diKbproperties.diph.dwHow = DIPH_DEVICE;
	diKbproperties.dwData = DIKEYBOARDBUFFERSIZE; // Arbitary buffer size
	keyboard->SetProperty( DIPROP_BUFFERSIZE, &diKbproperties.diph );

	keyboard->Acquire();
}

LPDIRECT3DDEVICE9 DXFactory::GetGraphicsDevice(){
	return this->d3dDevice;
}

LPD3DXSPRITE DXFactory::GetSprite(){
	return this->sprite;
}

LPDIRECTINPUTDEVICE8 DXFactory::GetKeyboard(){
	return this->keyboard;
}

DIPROPDWORD DXFactory::GetKeyboardProperties(){
	return this->diKbproperties;
}

