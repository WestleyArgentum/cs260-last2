#include "Window.hpp"
#include <mmsystem.h>
#include <shellapi.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Window Public Methods

/**************************************************************************************************/
/**************************************************************************************************/
Window::Window( const std::string &name, WindowProcedure proc, unsigned int width,
  unsigned int height ) : width_(width), height_(height), name_(name), title_(name), handle_(NULL)
{
    // Get the HINSTANCE of this application. (aka first param of WinMain)
  hInst_ = GetModuleHandle( NULL );

    // Setup all the variables that make up our window!
  window_.cbSize        = sizeof(WNDCLASSEX);
  window_.style         = CS_HREDRAW | CS_VREDRAW;
  window_.lpfnWndProc   = proc;
  window_.cbClsExtra    = 0;
  window_.cbWndExtra    = 0;
  window_.hInstance     = hInst_;
  window_.hIcon         = LoadIcon( hInst_, MAKEINTRESOURCE(IDI_APPLICATION) );
  window_.hCursor       = LoadCursor( NULL, IDC_ARROW );
  window_.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
  window_.lpszMenuName  = NULL;
  window_.lpszClassName = name_.c_str();
  window_.hIconSm       = LoadIcon( hInst_, MAKEINTRESOURCE( IDI_APPLICATION ) );
}

/**************************************************************************************************/
/**************************************************************************************************/
Window::~Window( void ) throw()
{
  if ( handle_ )
  {
    UnregisterClass( name_.c_str(), hInst_ );
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void Window::SetTitle( const std::string &title )
{
  title_ = title;
}

/**************************************************************************************************/
/**************************************************************************************************/
bool Window::Create( int show )
{
    // Register this window with the OS
  if ( !RegisterClassEx( &window_ ) )
  {
    return false;
  }

    // The create our window. Bam!
  handle_ = CreateWindow(
    name_.c_str(),                  // Application name.
    title_.c_str(),                 // Title bar text.
    WS_OVERLAPPEDWINDOW,            // Type of window to create.
    CW_USEDEFAULT, CW_USEDEFAULT,   // Initial position (x, y)
    width_, height_,                // Window size
    GetDesktopWindow(),             // Window's parent.
    NULL,                           // Menu bar.
    hInst_,                         // Application instance.
    NULL );                         // lpParam not used.

  if ( handle_ == NULL )
  {
    return false;
  }

    // Accept file drag and drop.
	DragAcceptFiles( handle_, TRUE );

    // The display this window with the presentation parameter ('show_') defined by the client and
    //  start having this window be updated.
  ShowWindow( handle_, show );
  UpdateWindow( handle_ );

  return true;
}

/**************************************************************************************************/
/**************************************************************************************************/
void Window::AddComponent( IWindowComponent *component )
{
  if ( component )
  {
    component->Create( handle_, hInst_ );
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
bool Window::Run( void )
{
    // Process all messages recieved by the OS for our window's created.
  while ( PeekMessage( &msg_, NULL, 0, 0, PM_REMOVE ) )
  {
    if ( msg_.message == WM_QUIT )
    {
      PostQuitMessage(0);

      return false;
    }

    TranslateMessage( &msg_ );
    DispatchMessage( &msg_ );
  }

  return true;
}

