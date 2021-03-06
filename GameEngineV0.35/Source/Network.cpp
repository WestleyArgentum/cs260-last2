#include "Precompiled.h"
#include "Network.h"
#include "GameProtocol.h"
#include "GameMessages.h"
#include <iostream>

namespace Framework
{
  ///Global pointer to the Network system.
  Network *NETWORK = NULL;

  ///30 second timeout phase.
  const double Network::CONNECTION_TIMEOUT = 15.0;

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
  
  bool Network::Connection::IsOutBasketEmpty( void )
  {
    for (unsigned type = 0; type < NMid::NumIds; ++type)
      if (outbasket[type].size())
        return false;

    return true;
  }

  bool Network::Connection::IsInBasketEmpty( void )
  {
    for (unsigned type = 0; type < NMid::NumIds; ++type)
      if (inbasket[type].size())
        return false;

    return true;
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

      remote.ResetData();

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
        else ///New connection.
        {
          Lock lock(mutex);

          ///Get the data from the socket.
          int num = socket->RetieveData(connections[remote].inbasket);

          ///If we couldn't extract any messages it was random data/connection
          if (num == 0)
            connections.erase(remote);

        }
      }
    } // end while (listening)
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
    ConnectionMap::iterator begin = connections.begin(), end = connections.end();
    while (begin != end)
    {
      if (begin->second.timer.TimeElapsed() > CONNECTION_TIMEOUT)
      {
        TimeoutMessage(begin->first).SendThis();
        connections.erase(begin++);
      }
      else
        ++begin;
    }
  }

  void Network::UpdateConnections( void )
  {
    ///Go through and send all messages to each connection.
    ConnectionMap::iterator begin = connections.begin(), end = connections.end();
    while (begin != end)
    {
      /// Tell the protocol which message basket to send from.
      if (!begin->second.IsOutBasketEmpty())
      {
        protocol->SetMessageList(begin->second.outbasket);
        socket->SendTo(begin->first, protocol);
        begin->second.EmptyOutBasket();
      }

      if (!begin->second.IsInBasketEmpty())
      {
        static Timer timer;
        std::cout << "Time since last message: " << timer.TimeElapsed() << std::endl;
        timer.Start();
        ///Send the messages we received out to the system.
        begin->second.DistributeMessages();
        begin->second.EmptyInBasket();
      }

      ++begin;
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
    thread_.Resume();
    Kill();

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
    RegisterProtocol(GameProtocol);
    protocol = NetAPI->GetProtocol("GameProtocol");
  }

  ///Builds up a buffer of messages to send to all connections.
  void Network::SendNetMessage( const INetMessage &m )
  {
    ///Add the message to all of the message queues.
    ConnectionMap::iterator begin = connections.begin(), end = connections.end();
    while (begin != end)
      begin++->second.outbasket[m.Type()].push_back(m.Clone());
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
    thread_.Resume();

    return true;
  }

  ///Attempts to find a server already running and connect to it.
  bool Network::FindServer()
  {
    InitializeSocket();

    //Build a connection message.
    ConnectionMessage msg;
    msg.name = NetAPI->Username();
    msg.address = socket->GetAddress();
    const NetAddress server = NetAPI->GetServerAddress();

    Connection &connection = connections[server];

    // add the broadcast connection to the connection list.
    connection.outbasket[NMid::Connection].push_back(&msg);
    protocol->SetMessageList(connection.outbasket);

    ///Send out the request for the server...
    socket->SendTo(NetAPI->GetServerAddress(), protocol);

    ///Clear the outbasket.
    connection.outbasket[NMid::Connection].clear();

    thread_.Resume();

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
