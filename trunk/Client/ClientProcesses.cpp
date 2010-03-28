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
void StartFileTransferProcess::operator()( const Command &command )
{
  if (client_->IsConnected())
    client_->SendFileRequest((const char*)command.data_, command.str_);
}

/**************************************************************************************************/
/**************************************************************************************************/
void SendFileTransferInfoProcess::operator()( const Command &command )
{
   // data is the user selected, str_ is the filename & path.
  if (client_->IsConnected())
    client_->HandleFileTransfer(command.data_);
}

/**************************************************************************************************/
/**************************************************************************************************/
void SendMessageProcess::operator() ( const Command &command )
{
  if (client_->IsConnected())
    client_->SendMsg(command.str_);
}

/**************************************************************************************************/
/**************************************************************************************************/
void ErrorBoxProcess::operator() ( const Command &command )
{
	MessageBox(0, command.str_.c_str(), "An error has occurred.", 0);
}
