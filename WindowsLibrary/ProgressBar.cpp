#include "ProgressBar.hpp"
#include <commctrl.h>       // PROGRESS_CLASS

const unsigned PROGRESS_WINDOW_WIDTH  = 180;
const unsigned PROGRESS_WINDOW_HEIGHT = 50;

/**************************************************************************************************/
/**************************************************************************************************/
ProgressBar::ProgressBar( const std::string &title ) : title_(title)
{
  HINSTANCE hInstance = GetModuleHandle( NULL );

  RECT fullWindow = { 0, 0, PROGRESS_WINDOW_WIDTH, PROGRESS_WINDOW_HEIGHT };
  AdjustWindowRect( &fullWindow, WS_OVERLAPPED, FALSE );

    // Setup all the variables that make up our window!
  window_.cbSize        = sizeof(WNDCLASSEX);
  window_.style         = CS_CLASSDC;
  window_.lpfnWndProc   = DefWindowProc;
  window_.cbClsExtra    = 0;
  window_.cbWndExtra    = 0;
  window_.hInstance     = hInstance;
  window_.hIcon         = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_APPLICATION) );
  window_.hCursor       = LoadCursor( NULL, IDC_ARROW );
  window_.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
  window_.lpszMenuName  = NULL;
  window_.lpszClassName = title_.c_str();
  window_.hIconSm       = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );

    // Register this window with the OS
  if ( !RegisterClassEx( &window_ ) )
  {
    return;
  }

    // The create our progress bar window.
  win_ = CreateWindow(
    title_.c_str(),                                 // Application name.
    title_.c_str(),                                 // Title bar text.
    WS_OVERLAPPED,                                  // Style of the window we are creating.
    CW_USEDEFAULT, CW_USEDEFAULT,                   // Initial position (x, y)
    PROGRESS_WINDOW_WIDTH, PROGRESS_WINDOW_HEIGHT,  // Window size
    GetDesktopWindow(),                             // Window's parent.
    NULL,                                           // Menu bar.
    hInstance,                                      // Application instance.
    NULL );                                         // lpParam not used.

    // Window not created...
  if ( win_ == NULL )
  {
    return;
  }

    // The display this window with the presentation parameter ('show_') defined by the client and
    //  start having this window be updated.
  ShowWindow( win_, SW_SHOWNORMAL );
  UpdateWindow( win_ );

    // Handle to our progress bar!
  pbHandle_ = CreateWindow(
    PROGRESS_CLASS,                       // Specify that we want a progress bar created.
    NULL,                                 // Caption text.
    WS_CHILD | WS_VISIBLE | PBS_SMOOTH,   // Progress bar styles
    5, 5,                                 // X and Y pos of the bar relative to the window
    165, 15,                              // Width / Height of the progress bar
    win_,                                 // Parent window!
    NULL,                                 // Progress Bar ID
    GetModuleHandle( NULL ),              // Handle to our application instance.
    NULL );                               // Parameters for main window

  SendMessage( pbHandle_, PBM_SETRANGE, 0, MAKELPARAM( 0, 100 ) );
  SendMessage( pbHandle_, PBM_SETSTEP, (WPARAM) 1, 0 );
}

/**************************************************************************************************/
/**************************************************************************************************/
ProgressBar::~ProgressBar( void ) throw()
{
  if ( win_ )
  {
    UnregisterClass( title_.c_str(), GetModuleHandle( NULL ) );
  }
  if ( pbHandle_ )
  {
    DestroyWindow( pbHandle_ );
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void ProgressBar::Step( void )
{
  SendMessage( pbHandle_, PBM_STEPIT, 0, 0 );
}

