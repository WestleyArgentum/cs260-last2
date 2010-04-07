
#include "precompiled.h"



#pragma comment(lib, "d3d9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "d3dx9.lib")

#include "MenuScreenView.h"
#include "MainMenuController.h"
#include "GameController.h"
#include "SpaceShipFactory.h"
#include "GameView.h"
#include "AsteroidFactory.h"
#include "DXFactory.h"

// turns on debug messages for mobiles
// #define MOBILE_DEBUG
// turns on debug messages for collisions
// #define COLLISION_DEBUG

// #define _CRTDBG_MAP_ALLOC

using namespace std;

//this is a crappy global but it's for the wndproc so there's really not a great way around it
MessageHandler* G_globalMessageHandler = NULL;
CRITICAL_SECTION wndprocsync;

#ifdef CONSOLE
void RedirectConsoleIO(void);
#endif

DWORD WINAPI StartGameLoop(void* param);

//entrypoint
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

#ifdef _CRTDBG_MAP_ALLOC
    int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
    _CrtSetDbgFlag(tmpFlag);
#endif

#ifdef CONSOLE
	RedirectConsoleIO();

	cout << "should see me on console" << endl;
#endif


	//windows overhead- create our window and stuff
	WindowFactory windowFactory(1024,768, MainWindProc);	
	RECT size;
	size.left = 0;
	size.top = 0;
	size.bottom = 768;
	size.right = 1024;

	//get a new window from the factory
	HWND window = windowFactory.GetWindow(hInstance);
	
	InitializeCriticalSection(&wndprocsync);

	//initialize the directx stuff
	DXFactory* dxFactory = new DXFactory(hInstance, window);
	dxFactory->Init();

	//display our window.  It should be doing whatever directx told it to
	ShowWindow(window, nShowCmd);

	//create the screen object for drawing
	Screen* screen = new Screen(window, size, dxFactory->GetGraphicsDevice());

	//create the models
	MenuModel* menuModel = new MenuModel();
	GameModel* gameModel = new GameModel();

	HRESULT res;
	
	LPDIRECT3DTEXTURE9 backgroundImage;
	res = D3DXCreateTextureFromFile(dxFactory->GetGraphicsDevice(), backgroundfilename, &backgroundImage);
	if(res != D3D_OK)
		throw new exception;

	

	//init views
	View* menuView = new MenuScreenView(screen, menuModel, dxFactory->GetGraphicsDevice());
	View* gameView = new GameView(screen, gameModel, dxFactory->GetGraphicsDevice());
	gameView->SetSprite(dxFactory->GetSprite());
	menuView->SetSprite(dxFactory->GetSprite());
	gameView->SetBackground(backgroundImage);
	menuView->SetBackground(backgroundImage);

	LPD3DXFONT gameScoreText;

	D3DXCreateFont(
		dxFactory->GetGraphicsDevice(), 14, 0, FW_NORMAL, 1, 
		false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, 
		"Arial", &gameScoreText);

	//init textures
	LPDIRECT3DTEXTURE9 temptext = NULL;
	res = 
		D3DXCreateTextureFromFileEx(dxFactory->GetGraphicsDevice(), "ships.jpg", D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(0,0,0), NULL, NULL, &temptext);

	if(FAILED(res))
		return 0;

	LPDIRECT3DTEXTURE9 asteroidText;

	res = D3DXCreateTextureFromFileEx(dxFactory->GetGraphicsDevice(), "space3.jpg", D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_A8R8G8B8, 
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(0,0,0), NULL, NULL, &asteroidText);

	if(FAILED(res))
		return 0;


	//rects for spaceships and such
	RECT spaceShipTextureRect;
	spaceShipTextureRect.top = 0;
	spaceShipTextureRect.left = 0;
	spaceShipTextureRect.bottom = 50;
	spaceShipTextureRect.right = 80;

	RECT asteroidTextureRect;
	asteroidTextureRect.top = 28;
	asteroidTextureRect.left = 380;
	asteroidTextureRect.bottom = 80;
	asteroidTextureRect.right = 440;


	//init factories	
	AsteroidFactory* afact = new AsteroidFactory(asteroidText, asteroidTextureRect);
	SpaceShipFactory* sfact = new SpaceShipFactory(temptext, spaceShipTextureRect);
	afact->SetFont(gameScoreText);
	

	//init controllers (these need views and factories sometimes so we should have them created already
	Controller* menuController = new MainMenuController(menuModel, menuView);
	Controller* gameController = new GameController(gameModel, gameView, afact, sfact);

	//init the main game object and add controllers and message handlers
	Game* game = new Game(screen, dxFactory);
	game->AddController(menuController, GameState::MainMenu);
	game->AddController(gameController, GameState::Playing);

	game->RegisterMessageHandler(GameState::MainMenu, G_globalMessageHandler);
	MessageHandler* DImessageHandler = new DInputKeyboardhandler(dxFactory->GetKeyboard());
	game->RegisterMessageHandler(GameState::Playing, DImessageHandler);

	// don't set this to 1 because the player's sprite always has ID 1
	IDFactory::ResetID(2);
//	_CrtSetBreakAlloc(25857);

	// start the game loop
	HANDLE messageThread = CreateThread(NULL, 0, StartGameLoop, game, 0, NULL);


	MSG msg;
	while(true){
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			if(msg.message == WM_QUIT)
				break;

		TranslateMessage( &msg );

		DispatchMessage( &msg );
		}
		if(WaitForSingleObject(messageThread, 5) == WAIT_OBJECT_0)
			PostQuitMessage(0);
   
   }
	backgroundImage->Release();
	temptext->Release();
	asteroidText->Release();
	gameScoreText->Release();


	delete DImessageHandler;
	delete G_globalMessageHandler;
	delete menuModel;
	delete menuView;
	delete menuController;

	delete gameController;
	delete gameModel;
	delete gameView;
	delete screen;
	delete game;
	delete dxFactory;
	delete afact;
	delete sfact;
	DeleteCriticalSection(&wndprocsync);

	/*PROFILER_UPDATE();
	PROFILER_OUTPUT();*/
	
#ifdef CONSOLE
	cout << endl << "Press any key to exit, any other key to continue";
	//cin.get();
	FreeConsole();
#endif
	
//	_CrtDumpMemoryLeaks();

	return 0;
}


//this is the actual game loop.  InitGameLoop() is blocking and returns if the game
//is over (i.e. the user quits).  Send the game a quit message to force it to quit
//at any time
DWORD WINAPI StartGameLoop(void* param){

	Game* game = (Game*)param;
	
	game->InitGameLoop();
	PostQuitMessage(0);

	
	return 0;



}

#ifdef CONSOLE
void RedirectConsoleIO(void)
{
	int hConHandle;

	long lStdHandle;

	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	FILE *fp;

	// allocate a console for this app

	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),

		&coninfo);

	coninfo.dwSize.Y = 500;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),

		coninfo.dwSize);

	// redirect unbuffered STDOUT to the console

	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "w" );

	*stdout = *fp;

	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console

	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "r" );

	*stdin = *fp;

	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console

	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "w" );

	*stderr = *fp;

	setvbuf( stderr, NULL, _IONBF, 0 );

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog

	// point to console as well

	ios::sync_with_stdio();

}
#endif