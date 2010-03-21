#include <iostream>

#include "WindowsLibrary/Window.hpp"          // Window
#include "WindowsLibrary/Console.hpp"         // CreateConsole()
#include "WindowsLibrary/FileDialogs.hpp"     // OpenFileDialog, SaveFileDialog

#include "ClientProcesses.hpp"

enum ButtonIDs
{
   BID_Send = 100   ///< Button id for sending a message to the server.
  ,BID_AcceptFile   ///< Button id for receiving a file from another client.
  ,BID_OpenFile     ///< Button id for opening a file to send to another client.
};    // enum ButtonIDs

namespace
{
  Textbox *sendbox = NULL;
}

/**************************************************************************************************/
/**************************************************************************************************/
LRESULT CALLBACK WinProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  HDC hdc;
  int id;

  std::string str;

  switch ( message )
  {
    case WM_PAINT:
      hdc = BeginPaint( hWnd, &ps );
      EndPaint( hWnd, &ps );
      break;

    case WM_DESTROY:
      PostQuitMessage( 0 );
      break;

    case WM_COMMAND:
      id = LOWORD( wParam );

      if ( id )
      {
        switch ( id )
        {
            // Send messages
          case BID_Send:
              // Get the message that the client would like to post.
            str = sendbox->GetText();
            sendbox->Clear();

              // Post this message so someone can process this message.
            CommandCenter->PostMsg( str, CID_SendMessage );
            break;

            // Accept a file.
          case BID_AcceptFile:
            {
              // TODO: Should error check to see if we have a file waiting to accept before we ask
              //  the user for the destination directory.

              SaveFileDialog dialog( hWnd );

              //if ( dialog.SaveFile( "Save as..." ) )    // Specify default name for the user
              if ( dialog.SaveFile() )
              {
                CommandCenter->PostMsg( dialog.GetFileName() + std::string( "\r\n" ), CID_Display );
              }
            }
            break;

            // Send a file.
          case BID_OpenFile:
            {
              // TODO: Should error check to see if the user is currently connected before prompting
              //  them to send a particular file.

              OpenFileDialog dialog( hWnd );

              //if ( dialog.OpenFile( "Save as..." ) )    // Specify default name for the user
              if ( dialog.OpenFile() )
              {
                CommandCenter->PostMsg( dialog.GetFileName() + std::string( "\r\n" ), CID_Display );
              }
            }
            break;
        }
      }
      else
      {
        switch ( wParam )
        {
          case BN_CLICKED:
            std::cout << "Unknown button pressed: #" << GetDlgCtrlID( (HWND)lParam ) << std::endl;
            break;
        }
      }
      break;

    case WM_DROPFILES:
      {
          // Query to see how many files were dropped onto our application window!
        unsigned count = DragQueryFile( (HDROP)wParam, 0xFFFFFFFF, 0, 0 );

        if ( count )
        {
          char buffer[MAX_PATH + 1] = {0};

          DragQueryFile( (HDROP)wParam, 0, buffer, sizeof(buffer) - 1 );
          DragFinish( (HDROP)wParam );

          CommandCenter->PostMsg( buffer + std::string( "\r\n" ), CID_Display );
        }
      }
      break;

    default:
      return DefWindowProc( hWnd, message, wParam, lParam );
      break;
  }

  return 0;
}

/**************************************************************************************************/
/**************************************************************************************************/
int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int nCmdShow )
{
    // Create a DebugConsole so we can print information for testing our chat program.
  CreateConsole();

    // Setup our window for our chat program.
  Window window( "CS260_ChatProgram!", WinProc );
  window.SetTitle( "CS260 Networking Chat Program!" );

    // Create our window with the display features given.
  window.Create( nCmdShow );

    // Setup our window with all the UI elements on the window.
  DWORD style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | WS_BORDER;
  Textbox text1( "", ComponentInfo( style, 10, 450, 650, 100 ) );
  sendbox = &text1;

  style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_READONLY | WS_BORDER;
  Textbox displaybox( "", ComponentInfo( style, 10, 10, 650, 430 ) );

  style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_BORDER;
  Button button1( "SEND!", BID_Send, ComponentInfo( style, 670, 450, 100, 100 ) );
  Button button2( "Send file...", BID_OpenFile, ComponentInfo( style, 670, 405, 100, 40 ) );
  Button button3( "Accept file...", BID_AcceptFile, ComponentInfo( style, 670, 360, 100, 40 ) );

  style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | LBS_NOSEL | LBS_SORT | WS_BORDER;
  Listbox userlistbox( "Listbox1", ComponentInfo( style, 670, 10, 100, 350 ) );

  window.AddComponent( &text1 );
  window.AddComponent( &displaybox );
  window.AddComponent( &button1 );
  window.AddComponent( &button2 );
  window.AddComponent( &button3 );
  window.AddComponent( &userlistbox );

  CommandCenter->RegisterProcess( new DisplayProcess( &displaybox ),     CID_Display );
  CommandCenter->RegisterProcess( new NewUserProcess( &userlistbox ),    CID_NewUser );
  CommandCenter->RegisterProcess( new RemoveUserProcess( &userlistbox ), CID_RemoveUser );


  sendbox->SetTextLimit( 255 );
  displaybox.SetText( "Welcome!\r\n"
    "To begin a chat session, please connect to the server by typing the command below. Thanks!\r\n"
    "\"/name=<username> /server:ip=<ip address> /server:port=<port number>\"\r\n\r\n" );

    // Finally start processing our window until our client decides to quit the chat program.
  while ( window.Run() ) {;}

  return window.ReturnCode();
}

