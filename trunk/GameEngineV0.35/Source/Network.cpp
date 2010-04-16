#include "Precompiled.h"
#include "Network.h"

namespace Framework
{
  ///30 second timeout phase.
  const double Network::CONNECTION_TIMEOUT = 30.0;

  Network::Network( void )
  {

  }

  Network::~Network( void )
  {

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
  void Network::SendNetMessage( INetMessage *m )
  {

  }

  ///Creates a socket and waits for connections.
  bool Network::HostServer( void )
  {
    return true;
  }

  ///Attempts to find a server already running and connect to it.
  bool Network::FindServer( void )
  {
    return true;
  }
}
