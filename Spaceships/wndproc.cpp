#include "wndproc.h"


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}
