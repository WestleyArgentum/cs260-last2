#ifndef SERVER_PROCESSES_H
#define SERVER_PROCESSES_H

#include "NetworkingLibrary/NetAPI.h" // NetAPI, TCPSOCKET, UDPSOCKET

// message queue
// client list
// methods on those two

// distribute message struct

typedef std::string ClientNick;

struct ClientData
{
  ClientNick name; ///< Client Nickname
  TCPSOCKET socket; ///< Handle to Client TCPSocket
};

class HostProcess
{
  TCPSOCKET listener;
  bool hosting;
public:
  HostProcess(const char *ip, unsigned port);
  ~HostProcess() { Quit(); }

  bool Host();
  void Quit();
};


#endif
