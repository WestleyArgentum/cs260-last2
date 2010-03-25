#pragma once

#include "WindowsLibrary/Threading.hpp"

#include <string>

class FileAccept : public RoutineObject
{
  public:
    FileAccept( const std::string &from, const std::string &file );

    bool Accepted( void );

  private:
    std::string from_;
    std::string file_;

      // Routine functions
    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void );
    virtual void FlushThread( void );

};    // FileAccept

