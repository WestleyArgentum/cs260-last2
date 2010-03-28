#include "ServerRoutines.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// ClientRoutine Protected Methods

/**************************************************************************************************/
/**************************************************************************************************/
void ClientRoutine::ProcessCommands()
{
  Lock lock(mutex);
  while (!commands.empty())
  {
    Command cmd = commands.front();
    commands.pop();
    switch (cmd.id_)
    {
    case CID_NewUser:
      socket->Send(NAPI::PT_ADD_NICK, cmd.str_.c_str(), cmd.str_.size());
      break;
    case CID_RemoveUser:
      socket->Send(NAPI::PT_DEL_NICK, cmd.str_.c_str(), cmd.str_.size());
      break;
    case CID_SendMessage:
      socket->Send(NAPI::PT_DATA_STRING, cmd.str_.c_str(), cmd.str_.size());
      break;
    }
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void ClientRoutine::SendFileTransferInfo( NAPI::PacketType pt, const FileTransferInfo &info )
{
  socket->Send(pt, &info, sizeof(FileTransferInfo));
}

/**************************************************************************************************/
/**************************************************************************************************/
void ClientRoutine::AddCommand(Command cmd)
{
  Lock lock(mutex);
  commands.push(cmd);
}

/**************************************************************************************************/
/**************************************************************************************************/
void ClientRoutine::InitializeThread( void )
{
   // ask for username
  socket->Send(NAPI::PT_REQ_NAME,0,0);
  for (Timer timeout; timeout.TimeElapsed() <  TIMEOUT_REQ_NAME;)
  {
    int ret = socket->Recieve();
    if (ret == SOCKET_ERROR)
      ;//Sleep(100); // wait for the user to respond...
    else if (socket->GetMsg().Type() == NAPI::PT_DATA_STRING)
    {
       // save the name in the routine and in the socket
      name = socket->GetMsg().DataToStr();
	    socket->SetID(name);
       // add user to the active users list so it can recieve commands.
      if (host->SetUserActive(GetID())) // if it couldn't be added, server is shutting down.
      {
         // inform the users of this new user
        CommandCenter->PostMsg(name, CID_NewUser);
        host->UpdateUserList(name);
		    socket->ToggleBlocking(false);
        running = true;
		    break;
      }
      else
      {
		    socket->ToggleBlocking(false);
        std::string msg( "Nickname already taken." );
        socket->Send(NAPI::PT_DATA_STRING, msg.c_str(), msg.size());
        host->SetUserInactive(this);
        break;
      }
    }
    else
    {
		  socket->ToggleBlocking(false);
      std::string msg( "Wrong packet sent." );
      socket->Send(NAPI::PT_DATA_STRING, msg.c_str(), msg.size());
        host->SetUserInactive(this);
      break;
    }
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void ClientRoutine::Run( void )
{
  // use a mutex inbetween calls to send and recieve on the socket
  while (running)
  {
    // check for recieving data
    int ret = socket->Recieve();

    if (ret == 0) ///< Socket disconnected, be done with it.
    {
      running = false;
      continue;
    }
    else if (ret != SOCKET_ERROR)
    {
      // socket didn't block, and got data, interpret it.
      std::string msg;
      switch (socket->GetMsg().Type())
      {
      case NAPI::PT_DATA_STRING:  ///< Message recieved.
         // Build the message and have the CommandCenter distribute it.
		  msg.assign(name.c_str()) += ": " + socket->GetMsg().DataToStr();
		  CommandCenter->PostMsg(msg, CID_Display);
		  CommandCenter->PostMsg(msg, CID_SendMessage);
        break;
      case NAPI::PT_ACCEPT_FILE:
      case NAPI::PT_REJECT_FILE:   // Forward the FileTransferInfo data to the requested user.
      case NAPI::PT_SEND_FILE:
        msg.assign(socket->GetMsg().Data());
        const FileTransferInfo *info = reinterpret_cast<const FileTransferInfo*>(socket->GetMsg().Data());
        host->ProcessFileTransferRequest( socket->GetMsg().Type(), *info );
        break;
      }
    }
     // socket blocked, process commands
    ProcessCommands();
  }
  CommandCenter->PostMsg(name, CID_RemoveUser);
}

/**************************************************************************************************/
/**************************************************************************************************/
void ClientRoutine::ExitThread( void ) throw()
{
  // Send fin message to client, unless an error has occured.
  host->SetUserInactive(this);
  NAPI::NetAPI->CloseSocket(socket);
  socket = 0;
}

/**************************************************************************************************/
/**************************************************************************************************/
void ClientRoutine::FlushThread( void )
{
  running = false;
  // TODO: maybe need to do other stuff...
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// HostRoutine Protected Methods / Static Data

ClientIDTag HostRoutine::idBase = 0;
/**************************************************************************************************/
/**************************************************************************************************/
bool HostRoutine::SetUserActive(ClientIDTag id)
{
  if (!hosting)
    return false;
  Lock lock(mutex);
  ClientRoutineMap::iterator client = pendingUsers.find(id);
  if (client != pendingUsers.end())
  {
    activeUsers[client->second->GetNick()] = client->second;
    pendingUsers.erase(client);
    return true;
  }
  return false;
}

/**************************************************************************************************/
/**************************************************************************************************/
bool HostRoutine::SetUserInactive(ClientRoutine *client)
{
  Lock lock(mutex);
  if (activeUsers.count(client->GetNick()))
  {
    inactiveUsers[client->GetID()] = client;
    activeUsers.erase(client->GetNick());
    return true;
  }
  else if (pendingUsers.count(client->GetID()))
  {
    inactiveUsers[client->GetID()] = client;
    pendingUsers.erase(client->GetID());
    return true;
  }
  return false;
}

/**************************************************************************************************/
/**************************************************************************************************/
void HostRoutine::DistributeMessage(Command cmd)
{
  Lock lock(mutex);
  ActiveUserMap::iterator begin = activeUsers.begin(), end = activeUsers.end();
  while (begin != end)
    begin++->second->AddCommand(cmd);
}

/**************************************************************************************************/
/**************************************************************************************************/
void HostRoutine::ProcessFileTransferRequest( NAPI::PacketType pt, const FileTransferInfo &info )
{
  std::string dest = info.user_;
  if (activeUsers.count(dest))
    activeUsers[dest]->SendFileTransferInfo(pt, info);
}

/**************************************************************************************************/
/**************************************************************************************************/
void HostRoutine::UpdateUserList(const std::string &name)
{
  // Add all the current users to the new client's userlist.
  Lock lock(mutex);
  ClientRoutine *client = activeUsers[name];
  ActiveUserMap::iterator begin = activeUsers.begin(), end = activeUsers.end();
  while (begin != end)
    if (begin->first != name)
      client->AddCommand(Command(CID_NewUser, begin++->first));
    else
      ++begin;
}

/**************************************************************************************************/
/**************************************************************************************************/
void HostRoutine::InitializeThread( void )
{
  // TODO: some exception checking later...
  listener = NAPI::NetAPI->NewTCPSocket("HostListener");
  listener->Bind(port);
  listener->Listen();
  hosting = true;
}

/**************************************************************************************************/
/**************************************************************************************************/
void HostRoutine::Run( void )
{
  while (hosting)
  {
    // listen for clients, create a ClientRoutine, send it on it's way.
    NAPI::TCPSOCKET contact = 0;
     // threading will catch exception thrown
    contact = listener->Accept();

     // Create the new ClientRoutine and give it an ID. It will add itself to active users later.
    ClientRoutine *client = new ClientRoutine(idBase++,contact,this);
    pendingUsers[client->GetID()] = client; // Add to the clientele, but not active users.
    client->BeginSession(); // Asks for username, then posts a message that the user has joined.
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void HostRoutine::ExitThread( void ) throw()
{
  // kick all users.
  hosting = false;
  ActiveUserMap::iterator aib = activeUsers.begin(), aie = activeUsers.end();
  while (aib != aie)
    aib++->second->EndSession();

    // clean up all data.
  ClientRoutineMap::iterator cib = inactiveUsers.begin(), cie = inactiveUsers.end();
  while (cib != cie)
    delete cib++->second;
}

/**************************************************************************************************/
/**************************************************************************************************/
void HostRoutine::FlushThread( void )
{
  // TODO: FIX THIS!!! WON'T TERMINATE!!!!
  hosting = false;
  thread_.Terminate();
}
