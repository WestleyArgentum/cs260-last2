#include "NetAPI.h"

namespace NAPI
{

/**************************************************************************************************/
/**************************************************************************************************/
bool operator==(const NetAddress &lhs, const NetAddress &rhs){
  return lhs.sin_family == rhs.sin_family && lhs.sin_port == rhs.sin_port &&
          lhs.sin_addr.s_addr == rhs.sin_addr.s_addr;
}

/**************************************************************************************************/
/**************************************************************************************************/
bool operator!=(const NetAddress &lhs, const NetAddress &rhs){
  return lhs.sin_family != rhs.sin_family && lhs.sin_port != rhs.sin_port &&
          lhs.sin_addr.s_addr != rhs.sin_addr.s_addr;
}

/**************************************************************************************************/
/**************************************************************************************************/
Error CreateError( Error::ErrorCode code )
{
  CHAR errstr[4096];
  int errcode = GetLastError();
  std::stringstream sstr;

  DWORD result = FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    errcode,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    errstr,
    sizeof(errstr),
    NULL );

  if ( result == 0 )
  {
    sstr << "Format message failed with: #" << GetLastError();

    return Error( Error::E_Unknown, sstr.str() );
  }

    // Append the error code into the message formated from windows!
  sstr << "Error #" << errcode << ": " << errstr;

  return Error( code, sstr.str() );
}

/**************************************************************************************************/
/**************************************************************************************************/
NetMessage::NetMessage(PacketType type, const void *dta, unsigned size, unsigned seq, unsigned ack)
{
  hdr.type = type;
  hdr.seq = seq;
  hdr.ack = ack;
  SetData(dta,size);
}

/**************************************************************************************************/
/**************************************************************************************************/
void NetMessage::SetData(const void *dt, unsigned sz)
{ memcpy(data,dt,hdr.data_size = (sz > MAX_MSG_SIZE ? MAX_MSG_SIZE : sz)); }

/**************************************************************************************************/
/**************************************************************************************************/
TCPSocket::TCPSocket() : bound(false), connected(false), rmsg(PT_INVALID), blocking(true) {}

/**************************************************************************************************/
/**************************************************************************************************/
TCPSocket::~TCPSocket() {}

/**************************************************************************************************/
/**************************************************************************************************/
bool TCPSocket::SetID(const std::string &id)
{ return NetAPI->UpdateID(this,id); }

/**************************************************************************************************/
/**************************************************************************************************/
bool TCPSocket::operator==(const TCPSocket &rhs) const
{ return socket == rhs.socket && id == rhs.id; }

/**************************************************************************************************/
/**************************************************************************************************/
int TCPSocket::ToggleBlocking(bool v) throw (Error)
{
  blocking = v;
  u_long val = (v ? 0 : 1);
  int ret = ioctlsocket(socket, FIONBIO, &val);
  if (ret == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
  }
  return ret;
}

/**************************************************************************************************/
/**************************************************************************************************/
int TCPSocket::Bind(unsigned port)
{
	if (bound)
		return -1;

	SecureZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr(NetAPI->LocalIP().c_str());

  // Check if unbindable socket.
	int ret = bind(socket, (sockaddr*)&address, sizeof(address));
  if (ret == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
	}

  ret = sizeof(address);
  if (getsockname(socket, (sockaddr*)&address, &ret) == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
  }

	bound = true;
	return 0;
}

/**************************************************************************************************/
/**************************************************************************************************/
int TCPSocket::Listen(unsigned num)
{
	int ret = listen(socket, num);
  if (ret == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
	}

	return ret;
}

/**************************************************************************************************/
/**************************************************************************************************/
int TCPSocket::Connect(const char *ip, unsigned port)
{
	if (connected || !bound)
		return -1;

  NetAddress remote;
	SecureZeroMemory(&remote, sizeof(remote));
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);
	remote.sin_addr.s_addr = inet_addr(ip);
	int ret = connect(socket, (sockaddr*)&remote, sizeof(remote));
  if (ret == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
	}

	connected = true;
	return 0;
}

/**************************************************************************************************/
/**************************************************************************************************/
int TCPSocket::Send(PacketType type, const void *data, unsigned size) const
{
  NetMessage msg(type, data, size);
  int ret = send(socket, (const char*)&msg, msg.Size(), 0);
  if (ret == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
	}

	return 0;
}

/**************************************************************************************************/
/**************************************************************************************************/
int TCPSocket::Recieve()
{
	int ret = recv(socket, reinterpret_cast<char*>(&rmsg), sizeof(rmsg), 0);
	if (ret == SOCKET_ERROR)
	{
		if (!blocking)
			return ret; // would have blocked

    Error er = CreateError(Error::E_SocketError);
    throw er;
	}

	return ret;
}

/**************************************************************************************************/
/**************************************************************************************************/
int TCPSocket::Close()
{
	if (!connected)
		return -1;

	connected = false;
	bound = false;
	shutdown(socket, SD_BOTH);
	closesocket(socket);
	return 0;
}

/**************************************************************************************************/
/**************************************************************************************************/
TCPSOCKET TCPSocket::Accept()
{
	int size;
  TCPSOCKET tcp = NetAPI->NewTCPSocket("DEFAULT");
	SecureZeroMemory(&tcp->address,size = sizeof(tcp->address));
	tcp->socket = accept(socket,(sockaddr*)&tcp->address,&size);
  if (tcp->socket == INVALID_SOCKET) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
  }

	return tcp;
}

/**************************************************************************************************/
/**************************************************************************************************/
UDPSocket::UDPSocket() : id("UNINITIALIZED SOCKET"), rmsg(PT_INVALID), blocking(true)
{ SecureZeroMemory(&address,sizeof(address)); }

/**************************************************************************************************/
/**************************************************************************************************/
bool UDPSocket::SetID(const std::string &id)
{ return NetAPI->UpdateID(this,id); }

/**************************************************************************************************/
/**************************************************************************************************/
int UDPSocket::ToggleBlocking(bool v) throw (Error)
{
  blocking = v;
  u_long val = (v ? 0 : 1);
  int ret = ioctlsocket(socket, FIONBIO, &val);
  if (ret == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
  }

  return ret;
}

/**************************************************************************************************/
/**************************************************************************************************/
///< Send data to the address specified.
int UDPSocket::SendTo(const NetAddress &remote, PacketType type, const void *data,
                      unsigned size, unsigned seq, unsigned ack) const throw (Error)
{
  NetMessage msg(type,data,size,seq,ack);
  int ret = sendto(socket, (const char *)&msg, msg.Size(), 0, (const sockaddr*)&remote, sizeof(remote));

  if (ret == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
  }

  return ret; // number of bytes sent.
}

/**************************************************************************************************/
/**************************************************************************************************/
///< Recv Data on the socket. Stores the address of the sender in address.
int UDPSocket::RecvFrom(NetAddress &remote) throw (Error)
{
  int sz = sizeof(remote);
  int ret = recvfrom(socket, (char*)&rmsg, sizeof(rmsg), 0, (sockaddr*)&remote, &sz);
  if (ret == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
  }

  return ret;
}

/**************************************************************************************************/
/**************************************************************************************************/
NetAPI_ *NetAPI_::GetInstance()
{
	static NetAPI_ inst;

	return &inst;
}

/**************************************************************************************************/
/**************************************************************************************************/
NetAPI_::NetAPI_() : init(false)
{}

/**************************************************************************************************/
/**************************************************************************************************/
NetAPI_::~NetAPI_()
{
	if (init)
		Cleanup();
}

/**************************************************************************************************/
/**************************************************************************************************/
// Returns 0 for success, WSAGetLastError() otherwise.
int NetAPI_::Init()
{
	if (init)
		return 0;

	// initialze winsock
	int ret = WSAStartup(MAKEWORD(2,2), &wsData);
  if (ret) {
    Error er = CreateError(Error::E_NetworkInit);
    throw er;
	}

	 // Save local IP address for later convience.
	localIP = inet_ntoa(*(in_addr*)*gethostbyname("")->h_addr_list);
	init = true;

	return 0;
}

/**************************************************************************************************/
/**************************************************************************************************/
void NetAPI_::Cleanup()
{
	// cleanup winsock
	if (init)
		WSACleanup();
}

/**************************************************************************************************/
/**************************************************************************************************/
std::string NetAPI_::LocalIP() const
{
	if (!init)
		return "Not Connected";

	return localIP;
}

/**************************************************************************************************/
/**************************************************************************************************/
TCPSOCKET NetAPI_::NewTCPSocket(const std::string &id) throw (Error)
{
	 // make sure there 
	std::string nid = id;
	for (int i = 0; tcp_sockets.count(nid); ++i)
		nid = id + char(i + '0');
	TCPSOCKET sock = new TCPSocket();
	sock->id = nid;
	SecureZeroMemory(&sock->address, sizeof(sock->address));
  sock->socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

  // make sure socket or WSASocket hasn't failed us.
  if (sock->socket == INVALID_SOCKET) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
	}

	Lock lock(mutex);
	tcp_sockets[sock->id] = sock;
	return sock;
}

/**************************************************************************************************/
/**************************************************************************************************/
UDPSOCKET NetAPI_::NewUDPSocket(const std::string &id) throw (Error)
{
	
  std::string nid = id;
  for (int i = 0; udp_sockets.count(nid); ++i)
	nid = id + char(i + '0');
  UDPSOCKET usock = new UDPSocket();
  usock->id = nid;
  usock->socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);

  // Make sure socket creation didn't fail.
  if (usock->socket == INVALID_SOCKET) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
  }

  // setup the address of the socket.
  SecureZeroMemory(&usock->address, sizeof(usock->address));
  usock->address.sin_addr.s_addr = inet_addr(LocalIP().c_str());
  usock->address.sin_family = AF_INET;
  usock->address.sin_port = 0;

  // Make sure bind doesn't fail.
  int ret = bind(usock->socket, (sockaddr*)&usock->address, sizeof(usock->address));
  if (ret == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
  }

  ret = sizeof(usock->address);
  if (getsockname(usock->socket, (sockaddr*)&usock->address, &ret) == SOCKET_ERROR) {
    Error er = CreateError(Error::E_SocketError);
    throw er;
  }

  Lock lock(mutex);
  udp_sockets[usock->id] = usock;
  return usock; // success! ^_^
}

/**************************************************************************************************/
/**************************************************************************************************/
UDPSOCKET NetAPI_::GetUDPSocket(const std::string &id)
{
	Lock lock(mutex);
	if (udp_sockets.count(id))
		return udp_sockets[id];

	throw "Error: socket doesn't exist.";
}

/**************************************************************************************************/
/**************************************************************************************************/
TCPSOCKET NetAPI_::GetTCPSocket(const std::string &id)
{
	Lock lock(mutex);
	if (tcp_sockets.count(id))
		return tcp_sockets[id];

	throw "Error: socket doesn't exist.";
}

/**************************************************************************************************/
/**************************************************************************************************/
bool NetAPI_::UpdateID(TCPSOCKET tcp, const std::string &newid)
{
  Lock lock(mutex);
  if (tcp_sockets.count(newid))
    return false;

  if (tcp_sockets.count(tcp->id)) {
    tcp_sockets.erase(tcp->id);
    tcp->id = newid;
    tcp_sockets[tcp->id] = tcp;
    return true;
  }

  return false;
}

/**************************************************************************************************/
/**************************************************************************************************/
bool NetAPI_::UpdateID(UDPSOCKET udp, const std::string &newid)
{
  Lock lock(mutex);
  if (udp_sockets.count(newid))
    return false;

  if (udp_sockets.count(udp->id)) {
    udp_sockets.erase(udp->id);
    udp->id = newid;
    udp_sockets[udp->id] = udp;
    return true;
  }

  return false;
}

/**************************************************************************************************/
/**************************************************************************************************/
int NetAPI_::CloseSocket(UDPSOCKET socket)
{
  Lock lock(mutex);
  udp_sockets.erase(socket->id);
  delete socket;
  return 0;
}

int NetAPI_::CloseSocket(TCPSOCKET socket)
{
  Lock lock(mutex);
  tcp_sockets.erase(socket->id);
  int ret = socket->Close();
  delete socket;
  return ret;
}

} // NAPI namespace
