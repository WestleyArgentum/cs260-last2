#pragma once

#include "NetworkingLibrary/NetAPI.h"
#include "WindowsLibrary/Threading.hpp"

#include <string>

typedef unsigned TransferID;

class IFileTransfer
{
  NAPI::NetAddress remote;
  NAPI::UDPSOCKET socket;

public:
  virtual bool IsDone( void ) = 0;
  virtual bool IsFail( void ) = 0;
  virtual void StartTransfer(const NAPI::NetAddress &remote) {}
  NAPI::NetAddress GetSocketInfo() { return socket->GetAdr(); }
};    // IFileTransfer

struct FileTransferInfo
{
  FileTransferInfo(TransferID id, const std::string &user, const std::string &file)
    : id_(id) { strcpy(user_, user.c_str()); strcpy(file_,file.c_str()); }

  TransferID id_;       ///< An ID to keep track of which transfer
  char user_[32];       ///< Username to send to.
  char file_[MAX_PATH]; ///< Filename to send.
  NAPI::NetAddress udp_;      ///< The UDP socket to communicate with.
};

class FileAccept : public RoutineObject, public IFileTransfer
{
  public:
    FileAccept( const std::string &from, const std::string &file );

    virtual bool IsDone( void );
    virtual bool IsFail( void );

  private:
    Mutex mutex_;

    std::string from_;
    std::string file_;

    int result_;
    bool done_;
    bool fail_;

     // Routine functions
    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void ) throw();
    virtual void FlushThread( void );

};    // FileAccept

class FileSend : public RoutineObject, public IFileTransfer
{
  public:
    FileSend( const std::string &to, const std::string &file );

    virtual bool IsDone( void );
    virtual bool IsFail( void );

  private:
    Mutex mutex_;

    std::string to_;
    std::string file_;

    bool done_;
    bool fail_;

      // Routine functions
    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void ) throw();
    virtual void FlushThread( void );
};    // FileSend

