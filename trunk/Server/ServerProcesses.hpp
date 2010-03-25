#ifndef SERVER_PROCESSES_H
#define SERVER_PROCESSES_H

#include "NetworkingLibrary/NetAPI.h" // NetAPI, TCPSOCKET, UDPSOCKET
#include "ServerRoutines.hpp"

// adds user to list, spawns new client process,
// informs all clients of the new user.
struct NewUserProcess : ICommandProcess
{
  NewUserProcess(HostRoutine &host) : host_(host) {;}
  virtual void operator()(const Command &command)
  { host_.UpdateUserList(command); host_.DistributeMessage(command); }

  HostRoutine &host_;
};

struct RemoveUserProcess : ICommandProcess
{
  RemoveUserProcess(HostRoutine &host) : host_(host) {;}
  virtual void operator()(const Command &command)
  { host_.DistributeMessage(command); }

  HostRoutine &host_;
};

struct SendMessageProcess : ICommandProcess
{
  SendMessageProcess(HostRoutine &host) : host_(host) {;}
  virtual void operator()(const Command &command)
  { host_.DistributeMessage(command); }

  HostRoutine &host_;
};


#endif
