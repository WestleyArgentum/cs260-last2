#pragma once

#include "WindowsLibrary/Threading.hpp"

#include <string>
#include <vector>
#include <hash_map>

enum CommandID
{
   CID_Unknown = 0
  ,CID_Display
  ,CID_NewUser
  ,CID_RemoveUser
  ,CID_SendFile
  ,CID_SendMessage
  ,CID_DistributeMessage

    // Keeps track of the total number of 
  ,CID_Total
};    // enum CommandID

struct Command
{
  Command( CommandID id = CID_Unknown, std::string str = "<null>", void *data = 0 )
    : id_(id), str_(str), data_(data) {;}

  CommandID id_;
  std::string str_;
  void *data_;
};    // struct Command

struct ICommandProcess
{
  virtual void operator()( const Command &command ) = 0;
};    // struct ICommandProcess

class CommandCenter_
{
  public:
    static CommandCenter_* GetInstance( void );

    void RegisterProcess( ICommandProcess *method, CommandID cid = CID_Unknown );

    void PostMsg( const std::string &message, CommandID cid );
    void PostCommand( const Command &command );

  private:
    typedef std::vector<ICommandProcess*>                   CommandProcesses;
    typedef stdext::hash_map< CommandID, CommandProcesses > CommandProcessMap;

      /// Map of all commands processes that only expect to recived one particular command.
    CommandProcessMap map_;
    CommandProcesses deletion_;

    //Mutex mutex_;

  private:
    CommandCenter_( void ) {;}
    ~CommandCenter_( void ) throw();

      // Not implemented! (Not needed...)
    CommandCenter_( const CommandCenter_ &rhs );
    CommandCenter_& operator=( const CommandCenter_ &rhs );

    CommandProcesses& GetProcessList( CommandID cid );
};    // class CommandCenter

#define CommandCenter CommandCenter_::GetInstance()

