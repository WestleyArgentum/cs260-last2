/**************************************************************************************************/
/*!
@file   CommandCenter.cpp
@author Robert Onulak
@author Justin Keane
@par    email: robert.onulak@@digipen.edu
@par    email: justin.keane@@digipen.edu

@par    Course: CS260
@par    Assignment #3

----------------------------------------------------------------------------------------------------
@attention � Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/
#include "CommandCenter.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// CommandCenter Public Methods

/**************************************************************************************************/
/**************************************************************************************************/
CommandCenter_* CommandCenter_::GetInstance( void )
{
  static CommandCenter_ center;

  return &center;
}

/**************************************************************************************************/
/**************************************************************************************************/
void CommandCenter_::RegisterProcess( ICommandProcess *method, CommandID cid )
{
  //Lock lock( mutex_ );

  deletion_.insert( method );

  if ( cid == CID_Unknown )
  {
      // Add this process to all command lists.
    for ( unsigned i = 1; i < CID_Total; ++i )
    {
      CommandProcesses &list = GetProcessList( static_cast<CommandID>(i) );

      list.insert( method );
    }
  }
  else
  {
    CommandProcesses &list = GetProcessList( cid );

    list.insert( method );
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void CommandCenter_::PostMsg( const std::string &message, CommandID cid, const void *data )
{
  Command command( cid, message, data );

  PostCommand( command );
}

/**************************************************************************************************/
/**************************************************************************************************/
void CommandCenter_::PostCommand( const Command &command )
{
    // Get the list to post this command to.
  CommandProcesses &list = GetProcessList( command.id_ );

    // Send a command to each of these methods.
  for ( CommandProcesses::iterator it = list.begin(); it != list.end(); ++it )
  {
    (**it)( command );
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// CommandCenter Private Methods

/**************************************************************************************************/
/**************************************************************************************************/
CommandCenter_::~CommandCenter_( void ) throw()
{
  for ( CommandProcesses::iterator it = deletion_.begin(); it != deletion_.end(); ++it )
  {
    delete *it;
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
CommandCenter_::CommandProcesses& CommandCenter_::GetProcessList( CommandID cid )
{
    // Return the list of the specific process list...
  return map_[cid];
}

