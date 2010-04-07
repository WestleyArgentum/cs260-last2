#pragma once
class Screen
{
public:
	Screen(HWND window, RECT screensize, LPDIRECT3DDEVICE9 device);
	~Screen(void);
	void SetWindowHandle(HWND window);
	HWND GetWindowHandle(HWND window);
	void GetBackBuffer();
	void UpdateScreen();
	RECT GetScreenSize();
	void Flush();
private:
	HWND _window;
	HBITMAP _screenbuffer;
	HBITMAP _backbuffer;
	HDC _deviceContext;
	HDC _compatibleDeviceContext;
	RECT _screensize;
	LPDIRECT3DDEVICE9 _device;
	LPD3DXSPRITE  _spriteInterface;
	int sceneCounter;
};
