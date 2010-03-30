#pragma once

#include "NetworkingLibrary/ChatProtocol.hpp"
#include "WindowsLibrary/Threading.hpp"
#include "FileSplitter.hpp"
#include "FileJoiner.hpp"

//#include <string>

const double TIMEOUT_FILE_TRANSFER = 60.0; ///< 1 minute timeout period
enum TransferType { TRANSFER_ACCEPT, TRANSFER_SEND };

class IFileTransfer
{
protected:
  NAPI::NetAddress remote;
  NAPI::UDPSOCKET socket;
  TransferType type;

public:
  IFileTransfer() { socket = NAPI::NetAPI->NewUDPSocket("UDP"); }
  virtual ~IFileTransfer() { NAPI::NetAPI->CloseSocket(socket); }

  virtual bool IsDone( void ) = 0;
  virtual bool IsFail( void ) = 0;
  virtual void Cancel( void ) = 0;
  virtual void Finish( void ) = 0;

  virtual TransferType Type( void ) const { return type; }
  NAPI::NetAddress GetSocketInfo() { return socket->GetAdr(); }
};    // IFileTransfer

class FileAccept : public RoutineObject, public IFileTransfer
{
  public:
    FileAccept(const FileTransferInfo &ftInfo );
    FileAccept( void ) { Cancel(); }

    virtual bool IsDone( void );
    virtual bool IsFail( void );
    virtual void Cancel( void );
    virtual void Finish( void );

  private:
    Mutex mutex_;
    NAPI::NetAddress remote_;
    TransferID id;

    std::string from_;
    std::string file_;

    __int64 size_;
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
    FileSend( const std::string &to, const std::string &from, unsigned id );
    ~FileSend( void );

    virtual void StartTransfer(const NAPI::NetAddress &remote);

    virtual bool IsDone( void );
    virtual bool IsFail( void );
    virtual void Cancel( void );
    virtual void Finish( void );

  private:
    Mutex mutex_;
    NAPI::NetAddress remote_;
    FileSplitter splitter;

    std::string to_;
    std::string path_;
    std::string file_;

    bool done_;
    bool fail_;

      // Routine functions
    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void ) throw();
    virtual void FlushThread( void );
};    // FileSend

