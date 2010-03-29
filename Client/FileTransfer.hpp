#pragma once

#include "NetworkingLibrary/ChatProtocol.hpp"
#include "WindowsLibrary/Threading.hpp"

//#include <string>

const double TIMEOUT_FILE_TRANSFER = 60.0; ///< 1 minute timeout period

class IFileTransfer
{
protected:
  NAPI::NetAddress remote;
  NAPI::UDPSOCKET socket;

public:
  IFileTransfer() { socket = NAPI::NetAPI->NewUDPSocket("UDP"); }
  virtual ~IFileTransfer() { NAPI::NetAPI->CloseSocket(socket); }

  virtual bool IsDone( void ) = 0;
  virtual bool IsFail( void ) = 0;
  virtual void Quit( void )  = 0;

  virtual void StartTransfer(const NAPI::NetAddress &remote) = 0;
  NAPI::NetAddress GetSocketInfo() { return socket->GetAdr(); }
};    // IFileTransfer

class FileAccept : public RoutineObject, public IFileTransfer
{
  public:
    FileAccept(const FileTransferInfo &ftInfo );

    virtual void StartTransfer(const NAPI::NetAddress &remote) {}

    virtual bool IsDone( void );
    virtual bool IsFail( void );
    virtual void Quit( void ) { fail_ = true; }

  private:
    Mutex mutex_;
    NAPI::NetAddress remote_;
    TransferID id;

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

    virtual void StartTransfer(const NAPI::NetAddress &remote);

    virtual bool IsDone( void );
    virtual bool IsFail( void );
    virtual void Quit( void ) { fail_ = true; }

  private:
    Mutex mutex_;
    NAPI::NetAddress remote_;

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

