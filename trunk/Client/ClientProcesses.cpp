#include "ClientProcesses.hpp"

/**************************************************************************************************/
/**************************************************************************************************/
void DisplayProcess::operator()( const Command &command )
{
	std::string msg = command.str_;
	msg.append("\r\n");
	displaybox_->AddText( msg );
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

void ErrorBoxProcess::operator() ( const Command &command )
{
	MessageBox(0, "ERROR", command.str_.c_str(), 0);
}
