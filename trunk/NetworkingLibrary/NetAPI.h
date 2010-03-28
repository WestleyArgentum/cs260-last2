#ifndef NETAPI__H
#define NETAPI__H

#undef UNICODE
#undef _UNICODE
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "winsock2.h"
#include <mswsock.h>
#pragma comment(lib, "ws2_32.lib")

#include "WindowsLibrary/Threading.hpp"

  // Removing the warning
  //  - C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning( disable : 4290 )

namespace NAPI ///< Networking API namespace
{

  // Maximum message size per packet.
  const unsigned MAX_MSG_SIZE = 1024;

  typedef std::string     Message;
  typedef std::string     IPAddress;
  typedef sockaddr_in     NetAddress;
  typedef unsigned short  Port;

  // LAME!!!!
  bool operator==(const NetAddress &lhs, const NetAddress &rhs);
  bool operator!=(const NetAddress &lhs, const NetAddress &rhs);

  struct Error : public std::exception
  {
    enum ErrorCode
    {
      E_Unknown = -1,   ///< Unknown error occured.
      E_NoMemory = 0,   ///< Failed to create some memory.
      E_NetworkInit,    ///< Failed to initialize our Network.
      E_SocketError     ///< Socket error while performing some operation.
    };    // enum ERROR

    Error( ErrorCode code, const Message &what ) throw() : code_(code), error_(0), what_(what) {;}

    virtual const char* what( void ) const throw() { return what_.c_str(); }

    ErrorCode code_;  ///< Error code given to this exception object by the user stating why it was thrown.
    int error_;       ///< Error number received from GetLastError()
    Message what_;    ///< String discribing what happened at the time this object was thrown.
  };    // struct Error

  Error CreateError(Error::ErrorCode code);

  /// Used to determine the type of message being sent.
  enum PacketType {
    // Special Messages
	  PT_INVALID,         ///< Packet is corrupt or some other error occured.
    PT_DIRECTED,        ///< Message is directed to a specific desitination, stored in the data.

    // Request Messages
	  PT_REQ_NAME,        ///< Requests a name from a User to reference the socket with.

    // Data Messages
	  PT_DATA_STRING,     ///< String Data. Used for messages.
    PT_DATA_PACKET,     ///< Plain Data packet, used in UDP

    // Chat Messages
	  PT_ADD_NICK,        ///< Message used to update client user list.
    PT_DEL_NICK,        ///< Message used to update client user list.

    //// File Messages
    //PT_SEND_FILE,       ///< Client informing sender it's ready to accept the file.
    //PT_REJECT_FILE,     ///< Client informing sender it's rejecting the file.
    //PT_ACCEPT_FILE,     ///< Client informing sender it's accepting the file.
  };

  class NetMessage
  {
    friend class UDPSocket;
    friend class TCPSocket;
	  /// Packet header used to send data between sockets. Add padding for UDP...
	  /// Sequence numbers and ACKs are stored in this. Separate for sizeof reasons
	  struct Header
	  {
      // TODO: Add source and destination fields.

		  PacketType type; ///< Type of packet being sent.
		  unsigned seq;    ///< Sequence number
		  unsigned ack;    ///< Acknoledgement number
		  short data_size; ///< Size of data appended.
	  } hdr; ///< NetMessage header.

	  char data[MAX_MSG_SIZE]; ///< Actual data to be sent.

  public:
	  NetMessage(PacketType t, const void *d = 0, unsigned s = 0, unsigned seq = 0, unsigned ack = 0);
    int Size() const { return sizeof(Header) + hdr.data_size; }
    int DataSize() const { return hdr.data_size; }
    const char *Data() const { return data; }
    std::string DataToStr() const { return std::string(Data(),DataSize()); }
    PacketType Type() const { return hdr.type; }
    unsigned GetSEQ() const { return hdr.seq; }
    unsigned GetACK() const { return hdr.ack; }

    void SetType(PacketType type) { hdr.type = type; }
	  void SetData(const void *data, unsigned size);
    void SetSEQ(unsigned seq) { hdr.seq = seq; }
    void SetACK(unsigned ack) { hdr.ack = ack; }
  };

  /// TCPSocket class. Used to send TCP messages between endpoints.
  class TCPSocket
  {
    friend class NetAPI_;
	  std::string id; // label, may make integer later
	  SOCKET socket; // actual socket
	  NetAddress address; // sock info
	  NetMessage rmsg; // stores the most recent packet recieved.
    bool blocking;

    // Only the NetAPI_ can create and destroy sockets.
	  TCPSocket();
	  ~TCPSocket();
  public:

    // Change the ID of the socket.
    bool SetID(const std::string &id);
    // Returns the ID of the socket.
    const std::string & GetID() const { return id; }

    const NetMessage & GetMsg() const { return rmsg; }
    const NetAddress & GetAdr() const { return address; }

	  bool operator==(const TCPSocket &rhs) const;

    int ToggleBlocking(bool val) throw (Error);
	  int Bind(unsigned port = 0) throw (Error);
	  int Listen(unsigned num = 10) throw (Error);
	  int Connect(const char *ip, unsigned port) throw (Error);
	  int Send(PacketType type, const void *data, unsigned size) const throw (Error);
	  int Recieve() throw (Error);
	  int Close();

	  TCPSocket * Accept();

	  bool bound;
	  bool connected;
  };

   // Handle to a TCPSocket.
  typedef TCPSocket* TCPSOCKET;

  /// The UDP_Socket class.
  class UDPSocket
  {
    friend class NetAPI_;
    std::string id;      ///< Type of socket.
    SOCKET socket;       ///< Actual socket.
    NetAddress address;  ///< Local address.
    NetMessage rmsg;     ///< Recieve message.
    bool blocking;

    // Only the NetAPI_ can create and destroy sockets.
    UDPSocket();
    ~UDPSocket() {}
  public:
     // Change the ID the socket is referenced by.
    bool SetID(const std::string &id);
     // Returns the ID of the socket.
    const std::string & GetID() const { return id; }
    
    const NetMessage & GetMsg() const { return rmsg; }
    const NetAddress & GetAdr() const { return address; }

     ///< Only need to compare IDs.
    bool operator==(const UDPSocket &rhs) const
    { return socket == rhs.socket && id == rhs.id; }

    int ToggleBlocking(bool val) throw (Error);

    ///< Send data to the address specified.
    int SendTo(const NetAddress &remote, PacketType type, const void *data, 
      unsigned size, unsigned seq, unsigned ack) const throw (Error);

    ///< Recv Data on the socket. Stores the address of the sender in address.
    int RecvFrom(NetAddress &remote) throw (Error);
  };

   // Handle to a UDPSocket
  typedef UDPSocket* UDPSOCKET;

  class NetAPI_
  {
	  Mutex mutex;
	  std::map<std::string, TCPSOCKET> tcp_sockets;
    std::map<std::string, UDPSOCKET> udp_sockets;
	  std::string localIP; // saved for convience.
	  WSADATA wsData;
	  bool init;

	  NetAPI_();
	  ~NetAPI_();
  public:
	  static NetAPI_ *GetInstance();

	   // Initializes WinSock
	  int Init();
	   // Shutdown WinSock
	  void Cleanup();

	   // returns the local IP address
	  std::string LocalIP() const;
	   // makes a new socket and returns it.
	  TCPSOCKET NewTCPSocket(const std::string &id) throw(Error);
     // makes a new UDP socket and initializes it.
    UDPSOCKET NewUDPSocket(const std::string &id) throw (Error);
	   // returns the socket with id from the socket_bank.
    TCPSOCKET GetTCPSocket(const std::string &id);
	   // returns the socket with id from the socket_bank.
	  UDPSOCKET GetUDPSocket(const std::string &id);

     // Change the ID the socket is referenced by.
    bool UpdateID(TCPSOCKET tcp, const std::string &newid);
    bool UpdateID(UDPSOCKET tcp, const std::string &newid);

	   // closes and deletes the socket
	  int CloseSocket(UDPSOCKET socket);
	  int CloseSocket(TCPSOCKET socket);
  };

  #define NetAPI NetAPI_::GetInstance()

} // NAPI namespace

#endif
