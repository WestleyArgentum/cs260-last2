#include "Client.hpp"
#include "WindowsLibrary/Timer.hpp"

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
void Client::InitializeThread( void )
{
  socket = NAPI::NetAPI->NewTCPSocket("ClientTCP");
  socket->Bind();
  socket->Connect(ip_.c_str(), port_);
  socket->ToggleBlocking(false); // socket can't be blocking or it locks up

  // TODO: FIX THE TIMER!!!!
  for (Timer timeout; timeout.TimeElapsed() < 5.0;)
  {
     // Send server the username.
    int ret = socket->Recieve();
    if (ret == SOCKET_ERROR)
      Sleep(1); // wouldblock
    else if (ret == 0)
      return; // connection broken
    else
    {
      if (socket->GetMsg().Type() == NAPI::PT_REQ_NAME)
        break;
      else
        return; // error occurred...
    }

  }

  socket->Send(NAPI::PT_DATA_STRING, name_.c_str(), name_.size());

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
    {
      // WOULDBLOCK
    }
    else if (ret == 0)
    {
      // disconnected...
    }
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::ExitThread( void )
{
  // TODO: Implemenet this!!
}

/**************************************************************************************************/
/**************************************************************************************************/
void Client::FlushThread( void )
{
  connected = false;
}
