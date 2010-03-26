#pragma once

#include "NetworkingLibrary/NetAPI.h"
#include "WindowsLibrary/Threading.hpp"
#include "WindowsLibrary/CommandCenter.hpp"

#include "FileTransfer.hpp"

class Client : public RoutineObject
{
  Mutex mutex;
  NAPI::TCPSOCKET socket; ///< TCP socket used to talk to the server.
  std::string name_;
  std::string ip_;
  unsigned port_;
  bool connected;
public:
  Client(const std::string &name) : socket(0), name_(name), port_(0), connected(false)
  { NAPI::NetAPI->Init(); }
  ~Client() { EndSession(); NAPI::NetAPI->Cleanup(); }

  void BeginSession(const std::string &ip, unsigned port);
  void SendCommand(const Command &command);
  bool IsConnected() const { return connected; }
  void EndSession();

protected:
      // Routine functions
    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void );
    virtual void FlushThread( void );
};
