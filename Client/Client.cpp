#include "Client.hpp"
#include "WindowsLibrary/Timer.hpp"
#include "WindowsLibrary/CommandCenter.hpp"

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
void Client::SendCommand(const Command &command)
{
  //Lock lock(mutex);
  switch (command.id_)
  {
  case CID_SendMessage:
    socket->Send(NAPI::PT_DATA_STRING, command.str_.c_str(), command.str_.size());
    break;
  }
}

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
  socket = NAPI::NetAPI->NewTCPSocket("ClientTCP");
  socket->Bind();
  socket->Connect(ip_.c_str(), port_);
  CommandCenter->PostMsg("Connected to server!", CID_Display);

  // TODO: FIX THE TIMER!!!!
  for (Timer timeout; timeout.TimeElapsed() < 5.0;)
  {
    int ret = socket->Recieve();
    if (ret == SOCKET_ERROR)
      ;//Sleep(10); // wouldblock
    else if (ret == 0)
	  {
	     // connection broken
	    CommandCenter->PostMsg("Connection broken.", CID_ErrorBox);
      return;
	  }
    else
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
    if (ret == SOCKET_ERROR)
      ;
    else if (ret == 0)
    {
      connected = false;
    }
    else
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
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::FlushThread( void )
{
  connected = false;
}
