#include "ProgressBar.hpp"
#include <commctrl.h>       // PROGRESS_CLASS

#pragma comment( linker, \
  "/manifestdependency:\"type='win32' " \
  "name='Microsoft.Windows.Common-Controls' " \
  "version='6.0.0.0' " \
  "processorArchitecture='*' " \
  "publicKeyToken='6595b64144ccf1df' " \
  "language='*'\"" )

const unsigned PROGRESS_WINDOW_WIDTH  = 180;
const unsigned PROGRESS_WINDOW_HEIGHT = 50;

/**************************************************************************************************/
/**************************************************************************************************/
ProgressBar::ProgressBar( const std::string &title ) : title_(title)
{
  HINSTANCE hInstance = GetModuleHandle( NULL );

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

  const unsigned screen_width  = GetSystemMetrics( SM_CXSCREEN );
  const unsigned screen_height = GetSystemMetrics( SM_CYSCREEN );

  const unsigned win_width     = 300;
  const unsigned win_height    = 140;

  const unsigned win_x_pos     = (screen_width  / 2) - (win_width  / 2);
  const unsigned win_y_pos     = (screen_height / 2) - (win_height / 2);

  win_ = CreateWindowEx(
    WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,
    title_.c_str(),
    title_.c_str(),
    WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
    win_x_pos, win_y_pos,
    win_width, win_height,
    WinSys->GetHwnd(),
    NULL,
    hInstance,
    NULL );

    // Window not created...
  if ( win_ == NULL )
  {
    return;
  }

  RECT winRect;
  GetClientRect( win_, &winRect );

  const int width  = winRect.right  - winRect.left;
  const int height = winRect.bottom - winRect.top;

  pbHandle_ = CreateWindowEx(
    WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,
    PROGRESS_CLASS,
    NULL,
    WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
    ( width - 200 ) / 2, ( ( height - 10 ) / 2 ) - 10,
    200, 20,
    win_,
    NULL,
    hInstance,
    NULL );

  SetRange( 0, 100 );
  SetStep( 1 );

    // The display this window with the presentation parameter ('show_') defined by the client and
    //  start having this window be updated.
  ShowWindow( win_, SW_SHOWNORMAL );
  UpdateWindow( win_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
ProgressBar::~ProgressBar( void ) throw()
{
  Destroy();
}

/**************************************************************************************************/
/**************************************************************************************************/
void ProgressBar::SetRange( unsigned short min, unsigned short max )
{
  SendMessage( pbHandle_, PBM_SETRANGE, 0, MAKELPARAM( min, max ) );
}

/**************************************************************************************************/
/**************************************************************************************************/
void ProgressBar::SetStep( unsigned short size )
{
  SendMessage( pbHandle_, PBM_SETSTEP, (WPARAM) 1, 0 );
}

/**************************************************************************************************/
/**************************************************************************************************/
void ProgressBar::Step( void )
{
  UINT before = SendMessage( pbHandle_, PBM_GETPOS, 0, 0 );

  SendMessage( pbHandle_, PBM_STEPIT, 0, 0 );

  UINT after = SendMessage( pbHandle_, PBM_GETPOS, 0, 0 );

  if ( after < before )   // Wrap around.
  {
    Destroy();
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void ProgressBar::Destroy( void )
{
  if ( win_ )
  {
    DestroyWindow( win_ );
    UnregisterClass( title_.c_str(), GetModuleHandle( NULL ) );
    win_ = NULL;
  }

  if ( pbHandle_ )
  {
    DestroyWindow( pbHandle_ );
    pbHandle_ = NULL;
  }
}

