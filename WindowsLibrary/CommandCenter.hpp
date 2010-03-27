#pragma once

#include "WindowsLibrary/Threading.hpp"

#include <string>
#include <set>
#include <hash_map>

////////////////////////////////////////////////////////////////////////////////////////////////////
// For Client:
//  > CID_Display
//    - Displays the command into the read only textbox of our chat program.
//  > CID_NewUser
//    - Command contains the username to add to the listbox. (list of online users)
//  > CID_RemoveUser
//    - Command contains the username to remove from the listbox. (list of online users)
//  > CID_SendFile
//    - Not implemented...
//  > CID_SendMessage
//    - Has the Client send a message to the server with the string provided.
//  > CID_ErrorMessage
//    - Creates a MessageBox with the text provided.
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// For Server:
//  > CID_Display
//    - Prints the message provided out to the command line.
//  > CID_NewUser
//  > CID_RemoveUser
//  > CID_SendFile
//  > CID_SendMessage
////////////////////////////////////////////////////////////////////////////////////////////////////

enum CommandID
{
   CID_Unknown = 0
  ,CID_Display
  ,CID_NewUser
  ,CID_RemoveUser
  ,CID_SendFile
  ,CID_SendMessage
  ,CID_ErrorBox

    // Keeps track of the total number of 
  ,CID_Total
};    // enum CommandID

struct Command
{
  Command( CommandID id = CID_Unknown, std::string str = "<null>", const void *data = 0 )
    : id_(id), str_(str), data_(data) {;}

  CommandID id_;
  std::string str_;
  const void *data_;
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

    void PostMsg( const std::string &message, CommandID cid, const void *data = 0 );
    void PostCommand( const Command &command );

  private:
    typedef std::set<ICommandProcess*>                      CommandProcesses;
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

