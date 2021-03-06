/**************************************************************************************************/
/*!
@file   Main.cpp
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
#include <iostream>

#include "Client.hpp"

#include "WindowsLibrary/Header.hpp"

#include "NetworkingLibrary/ConfigReader.hpp"
#include "ClientProcesses.hpp"

#include "FileSplitter.hpp"

enum ButtonIDs
{
   BID_Send = 100   ///< Button id for sending a message to the server.
  ,BID_OpenFile     ///< Button id for opening a file to send to another client.
};    // enum ButtonIDs

namespace
{
  Textbox *sendbox  = NULL;
  Listbox *userlist = NULL;
}

/**************************************************************************************************/
/**************************************************************************************************/
void SendMsg( const std::string &str )
{
  try
  {
      // Post this message so someone can process this message.
    if (!str.empty())
      CommandCenter->PostMsg( str, CID_SendMessage );
  }
  catch ( const NAPI::Error &e )
  {
    CommandCenter->PostMsg(
      "Connection to the server has been lost due to some unknown error.\r\n"
      "You will no longer be able to send messages. Please quit and try again.\r\n",
      CID_Display );
    DebugPrint( e.what() );
  }
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

    case WM_KEYDOWN:
      switch ( wParam )
      {
        case VK_RETURN:
            // Get the message that the client would like to post.
          str = sendbox->GetText();
          sendbox->Clear();

            // Post this message so someone can process this message.
          SendMsg(str);
          break;
      }
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

            SendMsg(str);

            break;

            // Send a file.
          case BID_OpenFile:
            {
              const char *selected = userlist->GetSelected();

              if ( selected )
              {
                 // Start the file transfer object, have it open a file dialog.
                CommandCenter->PostMsg( selected, CID_StartTransfer );
              }
              else
              {
                MessageBox( NULL, "Please select an user to send the file to.",
                  "Unknown destination!", MB_OK | MB_ICONEXCLAMATION );
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

          //CommandCenter->PostMsg( buffer + std::string( "\r\n" ), CID_Display );
        }
      }
      break;
  }

  return DefWindowProc( hWnd, message, wParam, lParam );
}

/**************************************************************************************************/
/**************************************************************************************************/
int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int nCmdShow )
{
    // Check for memory leaks
  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_16_DF );

    // Create a DebugConsole so we can print information for testing our chat program.
  //CreateConsole();    // Could also use DebugPrint!

  HINSTANCE temp = GetModuleHandle( NULL );

    // Setup our window for our chat program.
  WinSys->Create( "CS260_ChatProgram!", WinProc, nCmdShow, "CS260 Chat Program" );

    // Setup our window with all the UI elements on the window.
  DWORD style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | WS_BORDER;
  Textbox text1( "", ComponentInfo( style, 10, 450, 650, 100 ) );
  sendbox = &text1;

  style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_READONLY | WS_BORDER;
  Textbox displaybox( "", ComponentInfo( style, 10, 10, 650, 430 ) );

  style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_BORDER;
  Button button1( "SEND!", BID_Send, ComponentInfo( style, 670, 450, 100, 100 ) );
  Button button2( "Send file...", BID_OpenFile, ComponentInfo( style, 670, 405, 100, 40 ) );

  style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | LBS_NOSEL | LBS_SORT | WS_BORDER;
  Listbox userlistbox( "Listbox1", ComponentInfo( style, 670, 10, 100, 400 ) );
  userlist = &userlistbox;

  WinSys->AddComponent( &text1 );
  WinSys->AddComponent( &displaybox );
  WinSys->AddComponent( &button1 );
  WinSys->AddComponent( &button2 );
  WinSys->AddComponent( &userlistbox );

  sendbox->SetTextLimit( 255 );
  displaybox.SetText( "Welcome!\r\n\r\n" );

    // Load in the server port/ip and client's name.
  Config configuration( "..\\Data\\Config.txt" );

  //ProgressBar bar( "ThisIsAnAmazingTextFile.txt transfering..." );

  Client client(configuration.username_);
  CommandCenter->RegisterProcess( new DisplayProcess( &displaybox ),     CID_Display );
  CommandCenter->RegisterProcess( new NewUserProcess( &userlistbox ),    CID_NewUser );
  CommandCenter->RegisterProcess( new RemoveUserProcess( &userlistbox ), CID_RemoveUser );
  CommandCenter->RegisterProcess( new SendMessageProcess( &client ),      CID_SendMessage );
  CommandCenter->RegisterProcess( new StartFileTransferProcess( &client ), CID_StartTransfer );
  CommandCenter->RegisterProcess( new SendFileTransferInfoProcess( &client ), CID_SendFileTransferInfo );
  CommandCenter->RegisterProcess( new ErrorBoxProcess(), CID_ErrorBox );

  client.BeginSession(configuration.ip_, configuration.port_);

    // Finally start processing our window until our client decides to quit the chat program.
  while ( WinSys->Run() )
  {
    //bar.Step();
    //Sleep(100);
    //Sleep(120);
  }

  return WinSys->ReturnCode();
}

