#pragma once

#include "WindowsLibrary/Threading.hpp"

#include <string>

struct IFileTransfer
{
  virtual bool IsDone( void ) = 0;
};    // IFileTransfer

class FileAccept : public RoutineObject, IFileTransfer
{
  public:
    FileAccept( const std::string &from, const std::string &file );

    virtual bool IsDone( void );

  private:
    Mutex mutex_;

    std::string from_;
    std::string file_;

    int result_;
    bool done_;

      // Routine functions
    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void ) throw();
    virtual void FlushThread( void );

};    // FileAccept

class FileSend : public RoutineObject, IFileTransfer
{
  public:
    FileSend( const std::string &to, const std::string &file );

    virtual bool IsDone( void );

  private:
    Mutex mutex_;

    std::string to_;
    std::string file_;

    bool done_;

      // Routine functions
    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void ) throw();
    virtual void FlushThread( void );
};    // FileSend

