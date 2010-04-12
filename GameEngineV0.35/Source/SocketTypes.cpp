#include "Precompiled.h"
#include "SocketTypes.h"
#include "NetAPI.h"

namespace Framework
{
  
  ///The base for generating id's for all sockets
  unsigned ASocket::id_base = 0;

  /// Only allow derived classes and NetAPI to construct this.
  ASocket::ASocket( void ) : id(id_base++), rbuffer(0), sbuffer(0), blocking(true)
  {
    ///Keep send and recv buffers separate. Also allocate due to size.
    rbuffer = new char[MAX_MSG_SIZE];
    sbuffer = new char[MAX_MSG_SIZE];
  }

  /// Only allow destruction by the same two parties.
  ASocket::~ASocket( void ) throw( )
  {
    ///Cleanup
    delete [] rbuffer;
    delete [] sbuffer;
  }

  ///False means non-blocking, true means blocking.
  int ASocket::ToggleBlocking(bool v) throw (Error)
  {
    blocking = v;
    u_long val = (v ? 0 : 1);
    int ret = ioctlsocket(socket, FIONBIO, &val);
    if (ret == SOCKET_ERROR)
      ThrowError(Error::E_SocketError);

    return ret;
  }

  ///Initialize underlying ISocket.
  TCPSocket::TCPSocket( void ) : bound(false), connected(false) {;}

  ///Shutdown the socket and cease sending and receiving operations.
  TCPSocket::~TCPSocket( void ) throw( )
  {
    Close();
  }

  ///Binds the socket to the desired port. If 0 is specified and random port is selected.
  ///Returns 0 if everything went well. Returns -1 if already bound, throws if an error occured.
  int TCPSocket::Bind( unsigned port ) throw ( Error )
  {
    ///Don't bind to two ports... bad!!!
	  if (bound)
		  return -1;

    ///Setup the socket address and port to bind to.
    address.ClearMemory();
    address.SetPort(port );
    address.SetIP(NetAPI->LocalIP());

    // Check if unbindable socket.
    int ret = bind(socket, address, address.Size());
    if (ret == SOCKET_ERROR)
      ThrowError(Error::E_SocketError);

    int size = address.Size();
    if (getsockname(socket, address, &size) == SOCKET_ERROR)
      ThrowError(Error::E_SocketError);

	  bound = true;
	  return 0;
  }

  ///Sets the socket to be a listener socket. Waits for incoming connections.
  int TCPSocket::Listen( unsigned num ) throw ( Error )
  {
	  int ret = listen(socket, num);
    if (ret == SOCKET_ERROR)
      ThrowError(Error::E_SocketError);

	  return ret;
  }

  ///Connect to a specified IP and port, returns -1 if already connected or unbound. 
  ///Returns WSAEWOULDBLOCK if non-blocking and no data received. Throws for errors.
  int TCPSocket::Connect( const char *ip, unsigned port ) throw ( Error )
  {
    if (connected || !bound)
		  return -1;

    ///Create the remote address struct.
    NetAddress remote(ip,port);

    ///Attempt to connect to the remote host.
    int ret = connect(socket, remote, remote.Size());
    if (ret == SOCKET_ERROR) {
      if ( !blocking && WSAGetLastError() == WSAEWOULDBLOCK )
			    return ret; // would have blocked

      ///Something actually went wrong.
      ThrowError(Error::E_SocketError);
	  }

    ///We've connected successfully.
	  connected = true;
	  return 0;
  }

  ///Sends a message to the connected address, returns the number of bytes sent.
  int TCPSocket::Send( const INetMessage &msg ) const throw ( Error )
  {
    ///Don't send empty messages.
    if (msg.Size() == 0)
      return 0;

    ///Have the message serialize its data into the buffer
    msg.SerializeData(sbuffer, MAX_MSG_SIZE);
    int ret = send(socket, sbuffer, msg.Size(), 0);
    if (ret == SOCKET_ERROR) {
      if ( !blocking && WSAGetLastError() == WSAEWOULDBLOCK )
			  return ret; // would have blocked

      ///Something actually went wrong.
      ThrowError(Error::E_SocketError);
	  }

    ///Success
	  return ret;
  }

  ///Attempts to read data from its connection, returns number of bytes read.
  int TCPSocket::Recv( void ) throw ( Error )
  {
    ///Attempt to recieve data from the remote address.
	  int ret = recv(socket, rbuffer, MAX_MSG_SIZE, 0);
	  if (ret == SOCKET_ERROR) {
      if ( !blocking && WSAGetLastError() == WSAEWOULDBLOCK )
			  return ret; // would have blocked

      ///Something actually went wrong.
      ThrowError(Error::E_SocketError);
	  }

	  return ret;
  }

  ///Shutdown the socket and disable it.
  int TCPSocket::Close( void )
  {
    ///Disconnect from server and shutdown sending and recieving.
	  connected = false;
	  bound = false;
	  shutdown(socket, SD_BOTH);
	  closesocket(socket);
	  return 0;
  }

  ///Accepts incomming connections, creating and
  ///returning a socket to speak to the other side with.
  TCPSOCKET TCPSocket::Accept( void )
  {
	  int size = sizeof(sockaddr_in);
    TCPSOCKET tcp = NetAPI->NewTCPSocket();
    tcp->socket = accept(socket,tcp->address,&size);
    if (tcp->socket == INVALID_SOCKET)
      ThrowError(Error::E_SocketError);

	  return tcp;
  }

  ///Shutdown the socket.
  UDPSocket::~UDPSocket( void )
  {
    shutdown(socket, SD_BOTH);
  }

  ///Send data to the address specified. Returns the number of bytes sent.
  int UDPSocket::SendTo( const NetAddress &remote, const INetMessage &msg ) const throw ( Error )
  {
    msg.SerializeData(sbuffer, MAX_MSG_SIZE);
    int ret = sendto(socket, sbuffer, msg.Size(), 0, remote, remote.Size());
    if (ret == SOCKET_ERROR) {
      if ( !blocking && WSAGetLastError() == WSAEWOULDBLOCK )
        return ret; // would have blocked

      ///Something really went wrong.
      ThrowError(Error::E_SocketError);
    }

    return ret;
  }

  ///Receive Data on the socket. Stores the address of the sender in address.
  int UDPSocket::RecvFrom( NetAddress &remote ) throw ( Error )
  {
    int size = remote.Size();
    int ret = recvfrom(socket, rbuffer, MAX_MSG_SIZE, 0, remote, &size);
    if (ret == SOCKET_ERROR) {
      if ( !blocking && WSAGetLastError() == WSAEWOULDBLOCK )
        return ret; // would have blocked

      ///Something really went wrong.
      ThrowError(Error::E_SocketError);
    }

    ///Return the number of bytes read.
    return ret;
  }

} ///namespace Framework
