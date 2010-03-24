#pragma once

#include <windows.h>
#include <string>

class OpenFileDialog
{
  public:
    OpenFileDialog( HWND hWnd );

      // Opens the 'Open Dialog Box' which prompts the user to select the file to open.
    BOOL OpenFile( void );
    BOOL OpenFile( const std::string &filename );

    const char* GetFileName( void );    // Get the name of the file that the user wants to open.

  private:
    OPENFILENAME ofn_;
    HWND hWnd_;
    char filename_[MAX_PATH];
};    // class OpenFileDialog

class SaveFileDialog
{
  public:
    SaveFileDialog( HWND hWnd );

      // Opens the 'Save Dialog Box' which prompts the user to select the directory and filename
      //  for where to save this file at.
    BOOL SaveFile( void );
      // Default the initial filename for the user to the filename given.
    BOOL SaveFile( const std::string &filename );

    const char* GetFileName( void );    // Get the name of the file to save.

  private:
    OPENFILENAME ofn_;
    HWND hWnd_;
    char filename_[MAX_PATH];
};    // class SaveFileDialog

