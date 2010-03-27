#pragma once

#include "Window.hpp"
#include <string>

class ProgressBar
{
  public:
    ProgressBar( const std::string &title );
    ~ProgressBar( void ) throw();

    void Step( void );

  private:
    WNDCLASSEX window_;
    HWND win_;

    HWND pbHandle_;

    std::string title_;
};    // ProgressBar

