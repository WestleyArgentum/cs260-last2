/**************************************************************************************************/
/*!
@file   NetAPI.cpp
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #4

----------------------------------------------------------------------------------------------------
@attention © Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/

#include "Precompiled.h"
#include "NetAPI.h"

namespace Framework
{ 
  ///Singleton getter method...
  NetAPI_ *NetAPI_::GetInstance()
  {
	  static NetAPI_ inst;

	  return &inst;
  }

  ///Constructor
  NetAPI_::NetAPI_() : init(false) {;}

  ///Cleans up all the sockets and winsock
  NetAPI_::~NetAPI_()
  {
	  if (init)
		  Cleanup();
  }

  ///Initializes winsock and grabs the local ip address. Returns 0 for success.
  int NetAPI_::Init()
  {
    ///Don't init twice.
	  if (init)
		  return -1;

	  ///Initialze winsock
	  int ret = WSAStartup(MAKEWORD(2,2), &wsData);
    if (ret)
      ThrowError(Error::E_NetworkInit);

	  ///Save local IP address for later convience.
	  localIP = inet_ntoa(*(in_addr*)*gethostbyname("")->h_addr_list);

    ///Read in the range of UDP ports specified by the config file.
    Config config( "..\\Data\\Config.txt" );\
    unsigned low = config.range_.low_;
    unsigned high = config.range_.high_;

    ///Add all the ports to the bank.
    while (low <= high)
      ports.push_back(PortState(low++,false));

    ///Success
	  init = true;
	  return 0;
  }

  
  ///Shuts down all the sockets, cleans up memory and winsock.
  void NetAPI_::Cleanup()
  {
    ///Clean up all sockets
    SocketBank::iterator b = sockets.begin(), e = sockets.end();
    while (b != e)
      CloseSocket(b++->second);

	  ///Cleanup winsock
	  if (init)
		  WSACleanup();
  }

 
  ///Returns the local IP address..
  IPAddress NetAPI_::LocalIP() const
  {
	  if (!init)
		  return IPAddress("Not Connected");

	  return localIP;
  }

  ///Creates a new TCP socket, initalizes parts of it and returns it.
  TCPSOCKET NetAPI_::NewTCPSocket( void )
  {
    ///Lock the mutex because we're adding to the SocketBank.
	  //Lock lock(mutex);

    ///Create the socket class and give it a socket from winsock.
    ///Could cause a problem worthy of throwing an error so it
    ///isn't done in the constructor.
	  TCPSOCKET sock = new TCPSocket();
    sock->socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

    ///Make sure socket or WSASocket hasn't failed us.
    if (sock->socket == INVALID_SOCKET) {
      ///Socket won't be used, prevent memory leak.
      delete sock;
      ThrowError(Error::E_SocketError);
    }
    
    ///Save the actual socket address info into the address member.
    int size = sock->GetAddress().Size();
    if (getsockname(sock->socket, sock->address, &size) == SOCKET_ERROR)
      ThrowError(Error::E_SocketError);

    ///Success, add it to the bank.
    sockets[sock->GetID()] = sock;
	  return sock;
  }

  ///Creates a new UDP socket and binds it to a port. Ports are selected from
  ///a base set that is specified in the config file.
  UDPSOCKET NetAPI_::NewUDPSocket( void )
  {
    ///Keep it safe!
    //Lock lock(mutex);

    ///Find a useable port for the UDP socket.
	  unsigned port = 0, i = 0;
    for (; i < ports.size(); ++i) {
      if (!ports[i].second) {
        port = ports[i].first;
        ports[i].second = true;
        break;
      }
    }

    ///If no ports are available then we can't create a UDP socket properly.
    if (port == 0)
      throw "No ports available for UDP sockets...";

    ///Create the socket struct, and ask winsock for a socket.
    UDPSOCKET sock = new UDPSocket();
    sock->socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);

    // Make sure socket creation didn't fail.
    if (sock->socket == INVALID_SOCKET) {
      ///Socket won't be used, prevents memory leak.
      delete sock;
      ThrowError(Error::E_SocketError);
    }

    ///Add the socket to the bank.
    sockets[sock->id] = sock;

    //Setup the address of the socket and save the index of the port.
    sock->address.SetPort(port);
    sock->portIndex = i;

    // Make sure bind doesn't fail.
    int ret = bind(sock->socket, sock->address, sock->GetAddress().Size());
    if (ret == SOCKET_ERROR) {
      Error er = CreateError(Error::E_SocketError);
      throw er;
    }

    ///Save the actual socket address info into the address member.
    int size = sock->GetAddress().Size();
    if (getsockname(sock->socket, sock->address, &size) == SOCKET_ERROR)
      ThrowError(Error::E_SocketError);

    ///Success! ^_^
    return sock;
  }

  ///Retrieve a socket from the bank with specified id.
  UDPSOCKET NetAPI_::GetUDPSocket( SocketID id )
  {
    ///Keep it safe!
	  //Lock lock(mutex);

    ///We use dynamic_cast because it may not be the correct type.
	  if (sockets.count(id))
		  return dynamic_cast<UDPSOCKET>(sockets[id]);

    ///Socket wasn't found, this shouldn't happen.
	  throw "Error: socket doesn't exist.";
  }

  ///Retrieve a socket from the bank with specified id.
  TCPSOCKET NetAPI_::GetTCPSocket( SocketID id )
  {
    ///Keep it safe!
	  //Lock lock(mutex);

    ///We use dynamic_cast because it may not be the correct type.
	  if (sockets.count(id))
		  return dynamic_cast<TCPSOCKET>(sockets[id]);

    ///Socket wasn't found, this shouldn't happen.
	  throw "Error: socket doesn't exist.";
  }

  ///Closes a socket and removes it from the bank.
  void NetAPI_::CloseSocket(ASocket *socket)
  {
    //Lock lock(mutex);
    sockets.erase(socket->id);

    ///If the socket is a UDPSocket, set the unused flag of the port it used.
    UDPSOCKET udp = dynamic_cast<UDPSOCKET>(socket);
    if (udp)
      ports[udp->portIndex].second = false;

    delete socket;
  }

} // Framework namespace
