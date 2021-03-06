/**************************************************************************************************/
/*!
@file   Client.cpp
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #3

----------------------------------------------------------------------------------------------------
@attention � Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/

#include "Client.hpp"
#include "WindowsLibrary/Timer.hpp"
#include "WindowsLibrary/CommandCenter.hpp"
#include "NetworkingLibrary/ChatProtocol.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Client Public Methods


Client::~Client()
{
  EndSession();
  FileTransferList::iterator begin = transfers.begin(), end = transfers.end();

  while (begin != end)
  {
    delete (begin++)->second;
  }

  NAPI::NetAPI->Cleanup();
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::BeginSession(const std::string &ip, unsigned port)
{
  ip_ = ip;
  port_ = port;
  thread_.Resume();
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::SendMsg(const std::string &msg)
{
  try {
    socket->Send(NAPI::PT_DATA_STRING, msg.c_str(), msg.size());
  }
  catch (NAPI::Error &er) {
    CommandCenter->PostMsg(er.what(), CID_ErrorBox);
    // cleanup stuff somehow...
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::SendFileRequest(const std::string &user)
{
  Lock lock(mutex);
   // create a new file sending object, it waits for the signal to begin.
  FileSend *trans = new FileSend(user,name_,idbase);
   // add the database of transfers, referenced by id.
  transfers[idbase++] = trans;
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::SendFileTransferInfo( const void *info_ )
{
  try {
    socket->Send(NAPI::PT_DIRECTED, info_, sizeof(FileTransferInfo));
  }
  catch (NAPI::Error &er) {
    CommandCenter->PostMsg(er.what(), CID_ErrorBox);
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::EndSession()
{
  if (thread_.IsRunning())
	  Kill();
  else
	  thread_.Terminate();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Client Private Methods

/**************************************************************************************************/
/**************************************************************************************************/
void Client::HandleFileTransfer( const void *info_ )
{
  const FileTransferInfo *info = reinterpret_cast<const FileTransferInfo*>(info_);
  switch (info->type_)
  {
  case MT_SEND_FILE:
    {
      FileAccept *accept = new FileAccept(*info);
      transfers[idbase++] = accept;
    }
    break;
  case MT_ACCEPT_FILE:
    {
      if (transfers.count(info->id_) && transfers[info->id_]->Type() == TRANSFER_SEND)
      {
         // Cast to the correct type and start the transfer.
        static_cast<FileSend*>(transfers[info->id_])->StartTransfer(info->udp_);
      }
      else {
        FileTransferInfo response(info->id_, MT_INVALID_MSG, info->from_, info->to_,
                                  info->file_, info->filesize_, info->udp_);

        socket->Send(NAPI::PT_DIRECTED, &response, sizeof(response));
      }
    }
    break;
  case MT_REJECT_FILE:
    {
      if (transfers.count(info->id_))
        transfers[info->id_]->Cancel();
    }
    break;
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::MonitorFileTransfers( void )
{
  Lock lock(mutex);

   // Checks if any file transfers have finished or failed.
  FileTransferList::iterator begin = transfers.begin(), end = transfers.end();
  while (begin != end) 
  {
    if (begin->second->IsDone())
    {
      begin->second->Finish();
      delete begin->second;
      transfers.erase(begin++);
    }
    else if (begin->second->IsFail())
    {
      begin->second->Cancel();
      delete begin->second;
      transfers.erase(begin++);
    }
    else
      ++begin;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Client Protected Methods

/**************************************************************************************************/
/**************************************************************************************************/
void Client::InitializeThread( void )
{
  try {
    CommandCenter->PostMsg("Attempting to connect to the server...", CID_Display);
    socket = NAPI::NetAPI->NewTCPSocket("ClientTCP");
    socket->Bind();
    socket->Connect(ip_.c_str(), port_);
  }
  catch (NAPI::Error &er) {
    CommandCenter->PostMsg(er.what(), CID_ErrorBox);
    throw er;
  }

  CommandCenter->PostMsg("Connected to server!", CID_Display);

  // TODO: FIX THE TIMER!!!!
  for (Timer timeout; timeout.TimeElapsed() < 5.0;)
  {
    int ret = socket->Recieve();
    if (ret == 0)
	  {
	     // connection broken
	    CommandCenter->PostMsg("Connection broken.", CID_ErrorBox);
      return;
	  }
    else if (ret != SOCKET_ERROR)
    {
      if (socket->GetMsg().Type() == NAPI::PT_REQ_NAME)
      {
         // Send the server our Nickname.
        socket->Send(NAPI::PT_DATA_STRING, name_.c_str(), name_.size());
		    socket->ToggleBlocking(false); // socket can't be blocking or it locks up
        break;
      }
	    else // server sent wrong type of message
	    {
		    CommandCenter->PostMsg("No name request recieved.", CID_ErrorBox);
		    return;
	    }
    }

  }
  connected = true;
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::Run( void )
{
  while (connected)
  {
    int ret = socket->Recieve();
    if (ret == 0) // connection closed
    {
      CommandCenter->PostMsg("Connection lost.", CID_ErrorBox);
      connected = false;
    }
    else if (ret != SOCKET_ERROR) // data recieved!
    {
      switch (socket->GetMsg().Type())
      {
      case NAPI::PT_DATA_STRING:
        CommandCenter->PostMsg(socket->GetMsg().DataToStr(), CID_Display);
        break;
      case NAPI::PT_ADD_NICK:
        CommandCenter->PostMsg(socket->GetMsg().DataToStr(), CID_NewUser);
        break;
      case NAPI::PT_DEL_NICK:
        CommandCenter->PostMsg(socket->GetMsg().DataToStr(), CID_RemoveUser);
        break;
      case NAPI::PT_DIRECTED:
        HandleFileTransfer(socket->GetMsg().Data());
        break;
      } // switch
    } // else if (ret!= SOCKET_ERROR)

     // Check if any tranfers are done or failed...
    MonitorFileTransfers();
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::ExitThread( void )
{
  // TODO: Keep the File Transferring going...
  NAPI::NetAPI->CloseSocket(socket);
  socket = 0;
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::FlushThread( void )
{
  connected = false;
  quit_.Release();
}
