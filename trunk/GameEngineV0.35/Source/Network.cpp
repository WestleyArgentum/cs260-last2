#include "Precompiled.h"
#include "Network.h"

namespace Framework
{
	Network* NETWORK = NULL;

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
          ///Make sure we aren't corrupting data while adding messages.
          mutex.Acquire();
          ///Add messages from users/server to queue.
          mutex.Release();
        }
        else
        {
          ///This is a new connection, or extraneous data.

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

  Network::Network( void ) : socket(0) 
  {
    NetAPI->Init();

		ErrorIf(NETWORK != NULL, "Network already initialized!");
		NETWORK = this;
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

  }

  void Network::Initialize( void )
  {
  }

  ///Builds up a buffer of messages and sends then in the update phase.
  void Network::SendNetMessage( const INetMessage *m )
  {

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
