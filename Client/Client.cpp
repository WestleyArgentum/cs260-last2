#include "Client.hpp"
#include "WindowsLibrary/Timer.hpp"
#include "WindowsLibrary/CommandCenter.hpp"
#include "NetworkingLibrary/ChatProtocol.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Client Methods

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
      if (transfers.count(info->id_))
        transfers[info->id_]->StartTransfer(info->udp_);
      else {
        FileTransferInfo response(info->id_, MT_INVALID_MSG, info->from_, info->to_, info->file_, info->udp_);
        socket->Send(NAPI::PT_DIRECTED, &response, sizeof(response));
      }
    }
    break;
  case MT_REJECT_FILE:
    {
      if (transfers.count(info->id_))
        transfers[info->id_]->Quit();
    }
    break;
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::SendMsg(const std::string &msg)
{
  socket->Send(NAPI::PT_DATA_STRING, msg.c_str(), msg.size());
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::SendFileRequest(const std::string &user, const std::string &file)
{
   // create a new file sending object, it waits for the signal to begin.
  FileSend *trans = new FileSend(user,file);
   // add the database of transfers, referenced by id.
  transfers[idbase] = trans;
   // create a struct holding the correct data relating to the file transfer and send it out
  FileTransferInfo info(idbase++, MT_SEND_FILE, user, name_, file, trans->GetSocketInfo());
  socket->Send(NAPI::PT_DIRECTED, &info, sizeof(info));
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

/**************************************************************************************************/
/**************************************************************************************************/
void Client::InitializeThread( void )
{
  try {
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
      }
    }
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
}
