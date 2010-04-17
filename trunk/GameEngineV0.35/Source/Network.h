#pragma once

#include "System.h"
#include "NetAPI.h"
#include "Threading.hpp"
#include "Timer.h"

namespace Framework
{
  class Network : public ISystem, private RoutineObject
  {
    ///The max time to wait without hearing from a connection.
    static const double CONNECTION_TIMEOUT;

    ///In Connection, first is the timeout timer, second is all the messages received.
    typedef std::pair<Timer, MessageList> Connection;
    typedef std::map<ProtocolType, MessageList> MessageBoxMap;
    typedef std::map<NetAddress, Connection> ConnectionMap;
    typedef std::map<std::string, NetAddress> ConnectionIdMap;
    typedef std::map<std::string, MessageBoxMap> ConnectionOutBox;

    /// TODO: Maybe make separate channels for Chat and Game.
    UDPSOCKET socket;          ///< Connection to server/clients.
    ConnectionMap connections; ///< All connections.
    ConnectionIdMap idmap;     ///< Keeps track of which name is assosiated with what address.
    ConnectionOutBox outboxes; ///< The out boxes for all the users.
    Mutex mutex;               ///< Keep from overwriting data in message queue.
    MessageList messages;      ///< The bank of messages received.
    bool listening;            ///< Whether or not to continue listening.

    ///Threading methods.
    virtual void InitializeThread( void );
    virtual void Run( void );
    virtual void ExitThread( void ) throw();
    virtual void FlushThread( void );

    ///Pauses and Resumes listening.
    void Pause( void );
    void Resume( void );

    void InitializeSocket( void );
    void CheckForTimeouts( void );
  public:
    Network( void );
    ~Network( void );

    virtual std::string GetName( void ) { return "Network"; }

    ///Used for system messages only. These will not be put across the connection.
    virtual void SendMessage( Message *m );

    ///Sends all messages out to all connected parties.
    virtual void Update( float dt );
    
    virtual void Initialize( void );

    ///Builds up a buffer of messages to send to all connections.
    void SendNetMessage( const ProtocolType &ptype, const INetMessage &m );

    ///Builds up a buffer of messages to send to a specific connection.
    void SendNetMessage( const std::string &user, const ProtocolType &ptype, const INetMessage &m );

    ///Creates a socket and waits for connections.
    bool HostServer( void );

    ///Attempts to find a server already running and connect to it.
    bool FindServer( void );

    ///Closes the socket 
    void CloseConnection( void );

  };

  extern Network * NETWORK;
}
