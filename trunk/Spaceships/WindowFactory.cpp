#include "WindowFactory.h"

WindowFactory::WindowFactory(int winx, int winy, WNDPROC winproc)
{
	this->_winx = winx;
	this->_winy = winy;
	this->_winproc = winproc;
		
}

WindowFactory::~WindowFactory(void)
{
}

HWND WindowFactory::GetWindow(HINSTANCE instance){

	WNDCLASS wc = this->GenerateWindowClass(instance);

	HWND hWnd = CreateWindow( classname,
      "SpaceShips",
      WS_BORDER |WS_SYSMENU,	//WS_SYSMENU added .... i wanted that [x] button visible so i can close out of the app.
      0,
      0,
      this->_winx,
      this->_winy,
      NULL,
      NULL,
      instance,
      NULL
   );
	return hWnd;

}

WNDCLASS WindowFactory::GenerateWindowClass(HINSTANCE instance){

	WNDCLASS wcx;
	SecureZeroMemory(&wcx,sizeof(wcx));

	//wcx.cbSize = sizeof(wcx);	
	wcx.lpfnWndProc = _winproc;
	wcx.hInstance = instance;
	wcx.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wcx.hIcon = NULL;	
	wcx.lpszClassName = classname;
	
	
		
	RegisterClass(&wcx);


	return wcx;

}