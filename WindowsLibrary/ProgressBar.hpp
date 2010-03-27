#pragma once

#define NOMINMAX
#include "Window.hpp"
#include <string>

class ProgressBar
{
  public:
    ProgressBar( const std::string &title );
    ~ProgressBar( void ) throw();

    void SetRange( unsigned short min, unsigned short max );
    void SetStep( unsigned short step );

    void Step( void );

  private:
    void Destroy( void );

  private:
      // Main window info of the progress bar
    WNDCLASSEX window_;
    HWND win_;

      /// Handle to the progress bar (which is a child of the window.)
    HWND pbHandle_;

    std::string title_;
};    // ProgressBar

