#include "Client.hpp"

#include "WindowsLibrary/CommandCenter.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// FileAccept Methods

/**************************************************************************************************/
/**************************************************************************************************/
FileAccept::FileAccept( const std::string &from, const std::string &file ) : from_(from),
  file_(file), result_(0), done_(false)
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
void FileAccept::InitializeThread( void )
{
  char buffer[512] = {0};

  sprintf_s( buffer, sizeof(buffer),
    "Incoming file from %s.\n Did you want to accept the file transfer?", from_.c_str() );

  result_ = MessageBox( NULL, buffer, "File transfer pending...", MB_YESNO | MB_ICONINFORMATION );
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileAccept::Run( void )
{
  if ( result_ == IDNO )
  {
      // Don't perform the file transfer if the user didn't want to accept the transfer.
    return;
  }


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
FileSend::FileSend( const std::string &to, const std::string &file ) : to_(to), file_(file),
  result_(0), done_(false)
{
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
void FileSend::InitializeThread( void )
{
}

/**************************************************************************************************/
/**************************************************************************************************/
void FileSend::Run( void )
{
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
