#include "Client.hpp"

#include "WindowsLibrary/CommandCenter.hpp"
#include "WindowsLibrary/FileDialogs.hpp"
#include "WindowsLIbrary/ProgressBar.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// FileAccept Methods

/**************************************************************************************************/
/**************************************************************************************************/
FileAccept::FileAccept( const FileTransferInfo &ftInfo ) : remote_(ftInfo.udp_), id(ftInfo.id_),
from_(ftInfo.from_), file_(ftInfo.file_), result_(0), done_(false), fail_(false)
{
  thread_.Resume();
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileAccept::IsDone( void )
{
  Lock lock( mutex_ );

  return done_;
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileAccept::IsFail( void )
{
  Lock lock( mutex_ );

  return fail_;
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::InitializeThread( void )
{
  char buffer[512] = {0};

  sprintf_s( buffer, sizeof(buffer),
    "Incoming file from %s.\nDid you want to accept the file transfer?", from_.c_str() );

  result_ = MessageBox( NULL, buffer, "File transfer pending...", MB_YESNO | MB_ICONINFORMATION );
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::Run( void )
{
  if ( result_ == IDNO )
  {
      // File transfer rejected.
    CommandCenter->PostMsg("", CID_SendFile, &FileTransferInfo(id, MT_REJECT_FILE, from_,"","",GetSocketInfo()));
    return;
  }

  SaveFileDialog saveas( NULL );
  saveas.SetRelativeDir( "..\\Data\\dropfolder" );
    // Select the destination of where to save the file and to save the file as.
  if ( !saveas.SaveFile( file_ ) )
  {
      // User canceled to save the file at a particular location. File transfer rejected.
    CommandCenter->PostMsg("", CID_SendFile, &FileTransferInfo(id, MT_REJECT_FILE, from_,"","",GetSocketInfo()));
    return;
  }

   // save the new file name if they changed it.
  file_ = saveas.GetFileName();
   // put from as the sender, that way the server knows where to route it.
  FileTransferInfo info(id, MT_ACCEPT_FILE, from_, "", file_, GetSocketInfo());
  CommandCenter->PostMsg("", CID_TransferResponse, &info);

  ProgressBar progress(file_);

  for (unsigned i = 100; --i;)
    progress.Step(), Sleep(100);

  // start recieving file!

  // Connect to the client wanting to send us a file.
  // Download file.
  // Close connection.
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::ExitThread( void ) throw()
{
    // Setting done flag.
  Lock lock( mutex_ );

  done_ = true;
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::FlushThread( void )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// FileSend Methods

/**************************************************************************************************/
/**************************************************************************************************/
FileSend::FileSend( const std::string &to, const std::string &file )
: to_(to), file_(file), done_(false), fail_(false)
{
  //thread_.Resume();
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::StartTransfer(const NAPI::NetAddress &remote)
{
   // Save the udp socket info and start the transfer.
  remote_ = remote;
  thread_.Resume();
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileSend::IsDone( void )
{
  Lock lock( mutex_ );

  return done_;
}

/**************************************************************************************************/
/**************************************************************************************************/
bool FileSend::IsFail( void )
{
  Lock lock( mutex_ );

  return fail_;
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::InitializeThread( void )
{
  // read file into memory and break into chunks.
  // start sending data over UDP
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::Run( void )
{

  ProgressBar progress(file_);

  for (unsigned i = 100; --i;)
    progress.Step(), Sleep(100);
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::ExitThread( void ) throw()
{
    // Setting done flag.
  Lock lock( mutex_ );

  done_ = true;
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::FlushThread( void )
{
}
