#pragma once

#include <windows.h>

#include <string>       // std::string
#include <list>         // std::list<>

struct IWindowComponent
{
  virtual ~IWindowComponent( void ) throw() {;}
  virtual void Create( HWND hWnd, HINSTANCE hInstance ) = 0;
};    // struct IWindowComponent

const unsigned DEFAULT_WIDTH  = 800;
const unsigned DEFAULT_HEIGHT = 600;

class Window
{
  public:
    typedef LRESULT ( CALLBACK *WindowProcedure )( HWND, UINT, WPARAM, LPARAM );

  public:
    Window( const std::string &name, WindowProcedure proc, unsigned width = DEFAULT_WIDTH,
      unsigned height = DEFAULT_HEIGHT );
    ~Window( void ) throw();

    void SetTitle( const std::string &title );

    void AddComponent( IWindowComponent *component );

    bool Create( int show );
    bool Run( void );

    WPARAM ReturnCode( void ) const { return msg_.wParam; }

  private:
    unsigned width_;
    unsigned height_;

    std::string name_;
    std::string title_;

    WNDCLASSEX window_;
    MSG msg_;
    HWND handle_;
    HINSTANCE hInst_;
};    // class Window

  // Include all the window component objects as well...
#include "Components.hpp"

