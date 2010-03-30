/**************************************************************************************************/
/*!
@file   Components.hpp
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

#include "Window.hpp"     // IWindowComponent
#include "Threading.hpp"

struct ComponentInfo
{
  ComponentInfo( DWORD style, unsigned posx, unsigned posy, unsigned width, unsigned height );

  DWORD style_;
  unsigned posx_;
  unsigned posy_;
  unsigned width_;
  unsigned height_;
};    // struct ComponentInfo

class Textbox : public IWindowComponent
{
  public:
    Textbox( const std::string &name, const ComponentInfo &info );
    virtual HWND Create( HWND parent, HINSTANCE hInstance );
    virtual void Init( void );

    void SetTextLimit( unsigned limit );
    void SetText( const std::string &str );

    std::string GetText( void );
    void AddText( const std::string &str );

    void Clear( void );

  private:
    static LRESULT CALLBACK EditProc( HWND hWndEdit, UINT msg, WPARAM wParam, LPARAM lParam );

  private:
    std::string name_;
    ComponentInfo info_;
    unsigned limit_;

    HWND handle_;
    WNDPROC wndProc_;
};    // class Textbox

class Button : public IWindowComponent
{
  public:
    Button( const std::string &name, int id, const ComponentInfo &info );
    virtual HWND Create( HWND parent, HINSTANCE hInstance );

  private:
    std::string name_;    // Button name.
    ComponentInfo info_;
    int id_;

    HWND handle_;
};    // class Button

class Listbox : public IWindowComponent
{
  public:
    Listbox( const std::string &name, const ComponentInfo &info );
    virtual HWND Create( HWND parent, HINSTANCE hInstance );

    bool Exists( const std::string &str );
    void AddString( const std::string &str );
    void RemoveString( const std::string &str );

    const char* GetSelected( void );

  private:
    Mutex mutex_;

    std::string name_;
    ComponentInfo info_;

    HWND handle_;
    char selected_[33];   // Size of max username length. (32, 33 is accounting for the NULL)
};    // class Listbox
