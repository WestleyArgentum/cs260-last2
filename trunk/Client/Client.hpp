#pragma once

#include "NetworkingLibrary/NetAPI.h"
#include "WindowsLibrary/Threading.hpp"

#include "FileTransfer.hpp"

class Client : public RoutineObject
{
  NAPI::TCPSOCKET socket; ///< TCP socket used to talk to the server.
  std::string name_;
  std::string ip_;
  unsigned port_;
  bool connected;
public:
  Client(const std::string &name) : socket(0), name_(name), port_(0), connected(false) {;}
  ~Client() { EndSession(); }

  void BeginSession(const std::string &ip, unsigned port);
  void EndSession() { Kill(); }

protected:
      // Routine functions
    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void );
    virtual void FlushThread( void );
};
