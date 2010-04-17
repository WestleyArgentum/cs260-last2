#pragma once // Make sure this header is only included once.

#include "NetUtilities.h"
#include "INetMessage.h"
#include "IProtocol.h"
#include "DataStream.h"

namespace Framework
{
  ///Forward Declarations /Typedefs of a handle to TCPSockets and UDPSockets.
  typedef class TCPSocket* TCPSOCKET;
  typedef class UDPSocket* UDPSOCKET;

  ///Socket ID, used to keep track of sockets.
  typedef unsigned SocketID;

  class ASocket
  {
    ///Give the NetAPI_ the ability to create and setup the socket.
    friend class NetAPI_;
    static unsigned id_base;

  protected:
	  SocketID id;        ///< ID of the socket as it's stored in the NetAPI
	  SOCKET socket;      ///< Actual socket
	  NetAddress address; ///< Sock info
    int bytesread_;     ///< Stores the number of bytes read from receiving data.
    DataStream rstream; ///< Stream to hold data received in.
    DataStream sstream; ///< Stream to hold data being sent.
    bool blocking;      ///< Whether or not the socket blocks.

  protected:
    /// Only allow derived classes and NetAPI to construct this.
    ASocket( void );
    /// Only allow destruction by the same two parties.
    virtual ~ASocket( void ) throw();

  public:
    // Returns the ID of the socket.
    SocketID GetID( void ) const { return id; }

    ///Returns the NetAddress struct containing all the socket info.
    const NetAddress & GetAddress( void ) const { return address; }

    ///Comparison operator. only compares the id and type.
    bool operator==( const ASocket &rhs ) const { return id == rhs.id; }
    
    ///Toggle blocking on or off.
    int ToggleBlocking( bool val ) throw ( Error );
  };

  ///TCPSocket class. Used to send TCP messages between endpoints.
  class TCPSocket : public ASocket
  {
    ///Only NetAPI_ can create and destroy this class.
    friend class NetAPI_;

	  bool bound;     ///< If the socket is bound to a port.
	  bool connected; ///< If the socket is connected to remote host.

  protected:
    ///Only the NetAPI_ can create and destroy sockets.
	  TCPSocket( void );
	  virtual ~TCPSocket( void ) throw();

  public:
    ///Status Methods
    bool IsBound( void ) const { return bound; }
    bool IsConnected( void ) const { return connected; }

    ///Action Methods
    
    ///Binds the socket to the desired port. If 0 is specified and random port is selected.
	  int Bind( unsigned port = 0 ) throw ( Error );
	  int Listen( unsigned num = 10 ) throw ( Error );
	  int Connect( const char *ip, unsigned port ) throw ( Error );
	  int Send( const INetMessage &msg ) throw ( Error );
	  int Recv( void ) throw ( Error );
	  int Close( void );

    ///Accepts incoming connections, creating and
    ///returning a socket to speak to the other side with.
	  TCPSOCKET Accept( void );
  };

  ///The UDPSocket class. Wrapper for 
  class UDPSocket : public ASocket
  {
    ///Only NetAPI_ can create and destroy this class.
    friend class NetAPI_;

    unsigned portIndex; ///< An index to the UDP port being used by the socket.
  protected:
    ///Only the NetAPI_ can create and destroy UDP sockets.
    UDPSocket( void ) {;}
    virtual ~UDPSocket( void );

  public:
    ///Send data to the address specified.
    int SendTo( const NetAddress &remote, const IProtocol *protocol ) throw ( Error );

    ///Receive data on the socket. Stores the address of the sender in address.
    int RecvFrom( NetAddress &remote ) throw ( Error );

    ///Retrieves the messages from the buffer. Returns the number of messages received.
    int RetieveData( MessageList &messages );
  };
}
