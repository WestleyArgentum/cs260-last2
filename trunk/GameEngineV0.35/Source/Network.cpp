#include "Precompiled.h"
#include "Network.h"
#include "GameProtocol.h"

namespace Framework
{
  ///Global pointer to the Network system.
  Network *NETWORK = NULL;

  ///30 second timeout phase.
  const double Network::CONNECTION_TIMEOUT = 30.0;

  void Network::Connection::EmptyInBasket( void )
  {
    for (unsigned i = 0; i < NMid::NumIds; ++i)
    {
      MessageList::iterator begin = inbasket[i].begin(), end = inbasket[i].end();
      while (begin != end)
        delete *begin++;

      inbasket[i].clear();
    }
  }
  void Network::Connection::EmptyOutBasket( void )
  {
    for (unsigned i = 0; i < NMid::NumIds; ++i)
    {
      MessageList::iterator begin = outbasket[i].begin(), end = outbasket[i].end();
      while (begin != end)
        delete *begin++;

      outbasket[i].clear();
    }
  }

  void Network::Connection::DistributeMessages( void )
  {
    for (unsigned i = 0; i < NMid::NumIds; ++i)
    {
      MessageList::iterator begin = inbasket[i].begin(), end = inbasket[i].end();
      while (begin != end)
      {
        const_cast<INetMessage*>(*begin)->SendThis();
        delete *begin++;
      }

      inbasket[i].clear();
    }
  }

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
          socket->RetieveData(connections[remote].inbasket);

          ///Reset the timeout timer.
          connections[remote].timer.Start();
        }
        else ///Net connection.
        {
          Lock lock(mutex);

          ///Get the data from the socket.
          int num = socket->RetieveData(connections[remote].inbasket);

          ///If we couldn't extract any messages it was random data/connection
          if (num == 0)
            connections.erase(remote);

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

  void Network::UpdateConnections( void )
  {
    ///Go through and send all messages to each connection.
    ConnectionMap::iterator begin = connections.begin(), end = connections.end();
    while (begin != end)
    {
      /// Tell the protocol which message basket to send from.
      protocol->SetMessageList(begin->second.outbasket);
      socket->SendTo(begin->first, protocol);
      begin->second.EmptyOutBasket();

      ///Send the messages we received out to the system.
      begin->second.DistributeMessages();
      begin->second.EmptyInBasket();
    }
  }

  Network::Network( void ) : socket(0) 
  {
    ErrorIf(NETWORK!=NULL,"Network already initialized.");
    NETWORK = this;

    NetAPI->Init();
  }

  Network::~Network( void )
  {
    delete protocol;
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
    //Send all messages to connections.
    UpdateConnections();
  }

  void Network::Initialize( void )
  {
    //Hard coded for now.
    protocol = new GameProtocol();
  }

  ///Builds up a buffer of messages to send to all connections.
  void Network::SendNetMessage( const INetMessage &m )
  {
    ///Add the message to all of the message queues.
    ConnectionMap::iterator begin = connections.begin(), end = connections.end();
    while (begin != end)
      begin->second.outbasket[m.Type()].push_back(m.Clone());
  }

  ///Builds up a buffer of messages to send to a specific connection.
  void Network::SendNetMessage( const NetAddress &connection, const INetMessage &m )
  {
    ///Add the message to the requested connection.
    if (connections.count(connection))
      connections[connection].outbasket[m.Type()].push_back(m.Clone());
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
