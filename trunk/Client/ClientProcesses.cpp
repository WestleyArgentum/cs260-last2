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
void RejectFileProcess::operator()( const Command &command )
{
  if (client_->IsConnected())
    client_->SendMsg(NAPI::PT_REJECT_FILE, command.data_, sizeof(FileTransferInfo));
}
/**************************************************************************************************/
/**************************************************************************************************/
void AcceptFileProcess::operator()( const Command &command )
{
  if (client_->IsConnected())
    client_->SendMsg(NAPI::PT_ACCEPT_FILE, command.data_, sizeof(FileTransferInfo));
}

/**************************************************************************************************/
/**************************************************************************************************/
void SendFileProcess::operator()( const Command &command )
{
   // data is the user selected, str_ is the filename & path.
  if (client_->IsConnected())
    client_->SendFileRequest((const char*)command.data_, command.str_ );
}

/**************************************************************************************************/
/**************************************************************************************************/
void SendMessageProcess::operator() ( const Command &command )
{
  if (client_->IsConnected())
    client_->SendMsg(NAPI::PT_DATA_STRING, command.str_);
}

/**************************************************************************************************/
/**************************************************************************************************/
void ErrorBoxProcess::operator() ( const Command &command )
{
	MessageBox(0, command.str_.c_str(), "An error has occurred.", 0);
}
