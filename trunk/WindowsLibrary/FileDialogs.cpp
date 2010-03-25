#include "FileDialogs.hpp"

namespace
{
  char gCurrentDirectory[MAX_PATH];
}

/**************************************************************************************************/
/**************************************************************************************************/
OpenFileDialog::OpenFileDialog( HWND hWnd ) : hWnd_(hWnd)
{
  SecureZeroMemory( &ofn_, sizeof(ofn_) );
  SecureZeroMemory( &filename_, sizeof(filename_) );

    // Get the current directory of our process.
  GetCurrentDirectory( sizeof(gCurrentDirectory), gCurrentDirectory );

    //TODO: Change current directory to a "dropbox" folder defined later...
  ofn_.lpstrInitialDir = gCurrentDirectory;
  ofn_.lStructSize = sizeof(ofn_);
  ofn_.hwndOwner   = hWnd;
  ofn_.lpstrFilter = "All Files (*.*)\0*.*\0";
  ofn_.lpstrFile   = filename_;
  ofn_.nMaxFile    = MAX_PATH;
  ofn_.Flags       = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
}

/**************************************************************************************************/
/**************************************************************************************************/
BOOL OpenFileDialog::OpenFile( void )
{
    // Have the user open a file for us to load.
  BOOL result = GetOpenFileName( &ofn_ );

    // Set our directory back to the path of our process.
  SetCurrentDirectory( gCurrentDirectory );

  return result;
}

/**************************************************************************************************/
/**************************************************************************************************/
BOOL OpenFileDialog::OpenFile( const std::string &filename )
{
  if ( filename.size() < MAX_PATH )
  {
    strcpy( filename_, filename.c_str() );

    return OpenFile();
  }
  else
  {
    return FALSE;
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void OpenFileDialog::SetRelativeDir( const std::string &relative_dir )
{
  dir_ = gCurrentDirectory + ("\\" + relative_dir);

  ofn_.lpstrInitialDir = dir_.c_str();
}

/**************************************************************************************************/
/**************************************************************************************************/
const char* OpenFileDialog::GetFileName( void )
{
  return ofn_.lpstrFile;
}

/**************************************************************************************************/
/**************************************************************************************************/
SaveFileDialog::SaveFileDialog( HWND hWnd ) : hWnd_(hWnd)
{
  SecureZeroMemory( &ofn_, sizeof(ofn_) );
  SecureZeroMemory( &filename_, sizeof(filename_) );

    // Get the current directory of our process.
  GetCurrentDirectory( sizeof(gCurrentDirectory), gCurrentDirectory );

    //TODO: Change current directory to a "dropbox" folder defined later...
  ofn_.lpstrInitialDir = gCurrentDirectory;
  ofn_.lStructSize = sizeof(ofn_);
  ofn_.hwndOwner   = hWnd_;
  ofn_.lpstrFilter = "All Files (*.*)\0*.*\0";
  ofn_.lpstrFile   = filename_;
  ofn_.nMaxFile    = MAX_PATH;
  ofn_.Flags       = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
}

/**************************************************************************************************/
/**************************************************************************************************/
BOOL SaveFileDialog::SaveFile( void )
{
    // Have the user open a file for us to load.
  BOOL result = GetSaveFileName( &ofn_ );

    // Set our directory back to the path of our process.
  SetCurrentDirectory( gCurrentDirectory );

  return result;
}

/**************************************************************************************************/
/**************************************************************************************************/
BOOL SaveFileDialog::SaveFile( const std::string &filename )
{
  if ( filename.size() < MAX_PATH )
  {
    strcpy( filename_, filename.c_str() );

    return SaveFile();
  }
  else
  {
    return FALSE;
  }
}

/**************************************************************************************************/
/**************************************************************************************************/
void SaveFileDialog::SetRelativeDir( const std::string &relative_dir )
{
  dir_ = gCurrentDirectory + ("\\" + relative_dir);

  ofn_.lpstrInitialDir = dir_.c_str();
}

/**************************************************************************************************/
/**************************************************************************************************/
const char* SaveFileDialog::GetFileName( void )
{
  return ofn_.lpstrFile;
}

