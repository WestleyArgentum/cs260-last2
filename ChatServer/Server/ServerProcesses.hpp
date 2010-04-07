/**************************************************************************************************/
/*!
@file   ServerProcesses.hpp
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #3

----------------------------------------------------------------------------------------------------
@attention © Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/
#ifndef SERVER_PROCESSES_H
#define SERVER_PROCESSES_H

#include "NetworkingLibrary/NetAPI.h" // NetAPI, TCPSOCKET, UDPSOCKET
#include "ServerRoutines.hpp"


// adds user to list, spawns new client process,
// informs all clients of the new user.
struct NewUserProcess : public ICommandProcess
{
  NewUserProcess(HostRoutine &host) : host_(host) {;}
  virtual void operator()(const Command &command)
  { host_.DistributeMessage(command); }

  HostRoutine &host_;
};

struct RemoveUserProcess : public ICommandProcess
{
  RemoveUserProcess(HostRoutine &host) : host_(host) {;}
  virtual void operator()(const Command &command)
  { host_.DistributeMessage(command); }

  HostRoutine &host_;
};

struct SendMessageProcess : public ICommandProcess
{
  SendMessageProcess(HostRoutine &host) : host_(host) {;}
  virtual void operator()(const Command &command)
  { host_.DistributeMessage(command); }

  HostRoutine &host_;
};

struct DisplayProcess : ICommandProcess
{
	virtual void operator()( const Command &command )
	{ std::cout << command.str_ << std::endl; }
};

#endif
