#pragma once

#include "System.h"
#include "NetAPI.h"

namespace Framework
{
  class Network : public ISystem
  {
    ///The max time to wait without hearing from a connection.
    static const double CONNECTION_TIMEOUT;

    ///first is the connection id, second is the timeout timer.
    typedef std::pair<std::string, double> Connection;
    typedef std::map<NetAddress, Connection> ConnectionMap;

    ///Maybe make separate channels for Chat and Game.
    UDPSOCKET socket; ///< Connection to server/clients.

    ConnectionMap connections; ///< All connections.
  public:
    Network( void );
    ~Network( void );

    virtual std::string GetName( void ) { return "Network"; }

    ///Used for system messages only. These will not be put across the connection.
    virtual void SendMessage( Message *m );

    ///Sends all messages out to all connected parties.
    virtual void Update( float dt );
    
    virtual void Initialize( void );


    ///Builds up a buffer of messages and sends then in the update phase.
    void SendNetMessage( INetMessage *m );

    ///Creates a socket and waits for connections.
    bool HostServer( void );

    ///Attempts to find a server already running and connect to it.
    bool FindServer( void );

  };
}
