#include "Precompiled.h"
#include "Network.h"

namespace Framework
{
  ///Global pointer to the Network system.
  Network *NETWORK = NULL;

  ///30 second timeout phase.
  const double Network::CONNECTION_TIMEOUT = 30.0;

  void Network::InitializeThread( void )
  {

  }
  void Network::Run( void )
  {
    ///Listen continuously for packets from users/server.
    while (listening)
    {
      NetAddress remote;
      int ret = socket->RecvFrom( remote );

      ///If we received data and it's from someone we know.
      if (ret > 0)
      {
        ///If this isn't a new connection.
        if (connections.count(remote))
        {
          Lock lock(mutex);

          ///Save all the messages into the queue.
          socket->RetieveData(connections[remote].second);

          ///Reset the timeout timer.
          connections[remote].first.Start();
        }
        else ///Net connection.
        {
          Lock lock(mutex);

          ///Get the data from the socket.
          int num = socket->RetieveData(connections[remote].second);

          ///If we couldn't extract any messages it was random data/connection
          if (num == 0)
            connections.erase(remote);
          else ///Otherwise start the timer.
            connections[remote].first.Start();
        }
      }
    }
  }
  void Network::ExitThread( void ) throw()
  {
    ///Remove any data and cleanup.
  }
  void Network::FlushThread( void )
  {
    listening = false;
  }

  ///Pauses and Resumes listening.
  void Network::Pause( void )
  {
    mutex.Acquire();
  }
  void Network::Resume( void )
  {
    mutex.Release();
  }

  ///Create the actual socket in 
  void Network::InitializeSocket( void )
  {
    ///Don't make multiple sockets.
    if (!socket)
      socket = NetAPI->NewUDPSocket();

    ///Make sure socket is non-blocking.
    socket->ToggleBlocking(false);
  }

  void Network::CheckForTimeouts( void )
  {
    ///Check to see if any users have timed out...
  }

  Network::Network( void ) : socket(0) 
  {
    ErrorIf(NETWORK!=NULL,"Graphics already initialized.");
    NETWORK = this;

    NetAPI->Init();
  }

  Network::~Network( void )
  {
    CloseConnection();
    NetAPI->Cleanup();
  }

  ///Used for system messages only. These will not be put across the connection.
  void Network::SendMessage( Message *m )
  {
  }

  ///Sends all messages out to all connected parties.
  void Network::Update( float dt )
  {
    Lock lock(mutex);
    ///Check timers on all connections.
    CheckForTimeouts();

    ///Send all messages out to users.
    ConnectionOutBox::iterator begin = outboxes.begin(), end = outboxes.end();
    while (begin != end)
    {
      MessageBoxMap::iterator mb = begin->second.begin(), me = begin->second.begin();
      while (mb != me)
      {
        ///Get the protocol to send the messages with.
        IProtocol *protocol = NetAPI->GetProtocol(mb->first);
        if (protocol)
        {
          ///Set the MessageList for the protocol to use.
          protocol->SetMessageList(&mb->second);
          socket->SendTo(idmap[begin->first], protocol);
          protocol->ClearMessages();

          ///Delete all the messages when we're done sending them so we don't send them twice.
          MessageList::iterator lb = mb->second.begin(), le = mb->second.end();
          while (lb != le)
            delete *le++;

          //Empty the list.
          mb->second.clear();
        }
        ++mb; // Next!
      }
    }
  }

  void Network::Initialize( void )
  {
  }

  ///Builds up a buffer of messages to send to all connections.
  void Network::SendNetMessage( const ProtocolType &ptype, const INetMessage &m )
  {
    ///Add the message to all of the message queues.
    ConnectionOutBox::iterator begin = outboxes.begin(), end = outboxes.end();
    while (begin != end)
      begin++->second[ptype].push_back(m.Clone());
  }

  ///Builds up a buffer of messages to send to a specific connection.
  void Network::SendNetMessage( const std::string &user, const ProtocolType &ptype, const INetMessage &m )
  {
    ///Add the message to the requested connection.
    if (outboxes.count(user))
      outboxes[user][ptype].push_back(m.Clone());
  }

  ///Creates a socket and waits for connections.
  bool Network::HostServer( void )
  {
    InitializeSocket();

    ///Spawn new thread.

    return true;
  }

  ///Attempts to find a server already running and connect to it.
  bool Network::FindServer( void )
  {
    InitializeSocket();

    ///Spawn new thread.

    return true;
  }

  ///Closes the socket 
  void Network::CloseConnection( void )
  {
    if (socket)
      NetAPI->CloseSocket(socket);

    socket = 0;
  }

}
