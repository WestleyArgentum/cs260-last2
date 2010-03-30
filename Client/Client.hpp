/**************************************************************************************************/
/*!
@file   Client.hpp
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #3

----------------------------------------------------------------------------------------------------
@attention © Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/
#pragma once

#include "NetworkingLibrary/NetAPI.h"
#include "WindowsLibrary/Threading.hpp"
#include "WindowsLibrary/CommandCenter.hpp"

#include "FileTransfer.hpp"

class Client : public RoutineObject
{
  typedef std::map<TransferID, IFileTransfer*> FileTransferList;

  Mutex mutex;
  TransferID idbase;          ///< Base for generating ID's for transfers.
  FileTransferList transfers; ///< List of file transfers in progress.
  NAPI::TCPSOCKET socket;     ///< TCP socket used to talk to the server.
  std::string name_;          ///< Client Nickname
  std::string ip_;            ///< IP of server to connect to.
  unsigned port_;             ///< Port of server to connect to.
  bool connected;             ///< Whether the client is connected to the server.
  Event quit_;

public:
  Client( const std::string &name ) : idbase(0), socket(0), name_(name), port_(0), connected(false)
    { NAPI::NetAPI->Init(); }
  virtual ~Client();

  void BeginSession( const std::string &ip, unsigned port );
  void SendFileRequest( const std::string &user );
  void SendFileTransferInfo( const void *info );
  void SendMsg( const std::string &msg );
  bool IsConnected() const { return connected; }
  void EndSession();
 
private:
  void HandleFileTransfer( const void *info );
  void MonitorFileTransfers( void );

protected:
      // Routine functions
    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void );
    virtual void FlushThread( void );
};
