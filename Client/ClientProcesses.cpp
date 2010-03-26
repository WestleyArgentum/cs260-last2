#include "ClientProcesses.hpp"

/**************************************************************************************************/
/**************************************************************************************************/
void DisplayProcess::operator()( const Command &command )
{
  displaybox_->AddText( command.str_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
void NewUserProcess::operator()( const Command &command )
{
  listbox_->AddString( command.str_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
void RemoveUserProcess::operator()( const Command &command )
{
  listbox_->RemoveString( command.str_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
void SendFileProcess::operator()( const Command &command )
{
  //FileTransfer filetransfer( command.str_ );
}

void SendMessageProcess::operator() ( const Command &command )
{
  client_.SendCommand(command);
}