#ifndef SERVER_ROUTINES_H
#define SERVER_ROUTINES_H

#include "NetworkingLibrary/NetAPI.h"
#include "WindowsLibrary/Threading.hpp"
#include "WindowsLibrary/Timer.hpp"
#include "WindowsLibrary/CommandCenter.hpp"

#include <queue>

typedef std::string ClientNick; ///< Nickname.
typedef unsigned ClientIDTag;   ///< ID of client.

const double TIMEOUT_REQ_NAME = 5.0; ///< Max time to wait for a name request
const double TIMEOUT_REQ_DATA = 5.0; ///< Max time to wait for a data request

class HostRoutine; ///< Forward declaration.

class ClientRoutine : public RoutineObject
{
  typedef std::queue<Command> CommandQueue;

  Mutex mutex;
  CommandQueue commands;  ///< A queue of commands to be processed.
  ClientIDTag idtag;      ///< ID tag for reference later.
  ClientNick name;        ///< Nickname of the client.
  NAPI::TCPSOCKET socket; ///< Handle to a socket.
  HostRoutine *host;      ///< Pointer to the host.
  bool running;           ///< Whether or not a session is running.

  void ProcessCommands();
public:
  ClientRoutine(ClientIDTag tag, NAPI::TCPSOCKET contact, HostRoutine *hr) 
   : socket(contact), running(false), host(hr) {;}
  ~ClientRoutine() { EndSession(); }

  ClientIDTag GetID() const { return idtag; }
  ClientNick GetNick() const { return name; }

  void BeginSession() { thread_.Resume(); }
  void EndSession() { Kill(); }
  void AddCommand(Command cmd); ///< Used to inform client of things while in thread.

protected:
  virtual void InitializeThread( void );
  virtual void Run( void );
  virtual void ExitThread( void ) throw();
  virtual void FlushThread( void );
};

class HostRoutine : public RoutineObject
{
  friend class ClientRoutine;
  static ClientIDTag idBase; ///< source of client ID's
  typedef std::map<ClientNick, ClientRoutine*> ActiveUserMap;
  typedef std::map<ClientIDTag, ClientRoutine*> ClientRoutineMap;

  Mutex mutex;
  ActiveUserMap activeUsers;      ///< Map of active user's Nicknames corresponding to IDTags.
  ClientRoutineMap pendingUsers;  ///< The clients themselves.
  ClientRoutineMap inactiveUsers; ///< Clients that have left or failed to connect.
  NAPI::TCPSOCKET listener;
  unsigned port;
  bool hosting;

  bool SetUserActive(ClientIDTag id);
  bool SetUserInactive(ClientRoutine *client);
public:
  HostRoutine(unsigned port_) : listener(0), port(port_), hosting(false) {;}
  ~HostRoutine() { Quit(); }

  void DistributeMessage(Command cmd);
  void UpdateUserList(Command cmd);
  void Host() { thread_.Resume(); }
  void Quit() { Kill(); }

protected:
  virtual void InitializeThread( void );
  virtual void Run( void );
  virtual void ExitThread( void ) throw();
  virtual void FlushThread( void );
};

#endif
