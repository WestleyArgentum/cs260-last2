/***************************************
***************NOTE*********************
****************************************
This file is set in the project settings
to #include in every file so that is why
you don't see it anywhere else.  If you 
can't get it to build, then that's why.
**************************************/

// STL and windows stuff
#undef UNICODE
#undef _UNICODE
#define WIN32_LEAN_AND_MEAN

// should create a console window
#define CONSOLE

#include <windows.h>
#include <list>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <fstream>

#include <algorithm>
#include <sstream>
#include <math.h>
#include <crtdbg.h>
#include <queue>
// directx stuff
#include <d3dx9.h>
#include "d3d9.h"

//utils and stuff
#include "Debugging.h"
#include "SmartPtr.h"
#include "WindowFactory.h"
#include "MainWindProc.h"
#include "IDFactory.h"
#include "RandomNumberGenerator.h"
#include "Physics.h"
#include "DXFactory.h"

//game stuff
#include "Screen.h"
#include "Message.h"
#include "GameState.h"
#include "IRenderable.h"

// model view controller
#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "assert.h"
#define D3DFVF_SPACESHIPSVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define WORLD_SIZE_X 1500
#define WORLD_SIZE_Y 1500

#ifdef CONSOLE
#endif


// turns off asteroids
// #define NO_ASTEROIDS	
// #define LOGGING
// #define ONE_ASTEROID

