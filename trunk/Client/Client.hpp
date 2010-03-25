#pragma once

#include "WindowsLibrary/Threading.hpp"
//#include "NetworkingLibrary/NetAPI.h"

#include "FileTransfer.hpp"

//
//class Client : public RoutineObject
//{
//  NAPI::TCPSOCKET socket; ///< TCP socket used to talk to the server.
//public:
//  Client();
//  ~Client() { EndSession(); }
//
//  bool BeginSession() { thread_.Resume(); }
//  void EndSession() { Kill(); }
//
//protected:
//      // Routine functions
//    virtual void InitializeThread( void );
//    virtual void Run( void );
//    virtual void ExitThread( void );
//    virtual void FlushThread( void );
//};
