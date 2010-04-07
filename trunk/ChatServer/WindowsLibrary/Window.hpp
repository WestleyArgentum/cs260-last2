/**************************************************************************************************/
/*!
@file   Window.hpp
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #3

----------------------------------------------------------------------------------------------------
@attention © Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/
#pragma once

#include <windows.h>

#include <string>         // std::string
#include <map>

struct IWindowComponent
{
  virtual ~IWindowComponent( void ) throw() {;}
  virtual HWND Create( HWND parent, HINSTANCE hInstance ) = 0;

  virtual void Init( void ) {;}
};    // struct IWindowComponent

const unsigned DEFAULT_WIDTH  = 800;
const unsigned DEFAULT_HEIGHT = 600;

class Window
{
  public:
    static Window* GetInstance( void );

    bool Create( const std::string &name, WNDPROC proc, int show = SW_SHOWNORMAL,
      const std::string &title = "" /*name*/, unsigned width = DEFAULT_WIDTH,
      unsigned height = DEFAULT_HEIGHT );

    HWND GetHwnd( void ) const { return handle_; }

    void AddComponent( IWindowComponent *component );
    IWindowComponent* GetComponent( HWND component );

    template < typename ComponentType >
    ComponentType HasComp( HWND component )
    {
      return static_cast<ComponentType>( GetComponent( component ) );
    }

    bool Run( void );
    WPARAM ReturnCode( void ) const { return msg_.wParam; }

  private:
    Window( void );
    ~Window( void ) throw();

      // Not implemented
    Window( const Window &rhs );
    Window& operator=( const Window &rhs );

  private:
    unsigned width_;
    unsigned height_;

    std::string name_;
    std::string title_;

    WNDCLASSEX window_;
    DWORD style_;
    MSG msg_;
    HWND handle_;
    HINSTANCE hInst_;

    typedef std::map< HWND, IWindowComponent* > ComponentMap;
    ComponentMap map_;
};    // class Window

#define WinSys Window::GetInstance()

  // Include all the window component objects as well...
#include "Components.hpp"

