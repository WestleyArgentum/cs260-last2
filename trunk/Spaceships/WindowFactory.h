#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif



//const int windowX = 800;
//const int windowY = 600;
const LPCSTR classname = "spaceships_class";

class WindowFactory
{
public:
	WindowFactory(int winx, int winy, WNDPROC winproc);
	HWND GetWindow(HINSTANCE instance);
	~WindowFactory(void);

protected:
	WNDCLASS GenerateWindowClass(HINSTANCE instance);

private:
	int _winx;
	int _winy;
	WNDPROC _winproc;
};
