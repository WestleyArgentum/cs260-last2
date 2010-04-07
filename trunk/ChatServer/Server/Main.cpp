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
@attention © Copyright 2010: DigiPen Institute of Technology (USA). All Rights Reserved.
*/
/**************************************************************************************************/

//#include "WindowsLibrary/CommandCenter.hpp"
#include "NetworkingLibrary/NetAPI.h"
#include "ServerProcesses.hpp"
#include "NetworkingLibrary/ConfigReader.hpp"

int main(int argc, char *argv[])
{
  using namespace NAPI;
  // initialize NetAPI
  NetAPI->Init();
  // read in port from config file
  Config configuration( "..\\Data\\Config.txt" );

  HostRoutine host(configuration.port_);
  CommandCenter->RegisterProcess(new SendMessageProcess(host), CID_SendMessage);
  CommandCenter->RegisterProcess(new NewUserProcess(host), CID_NewUser);
  CommandCenter->RegisterProcess(new RemoveUserProcess(host), CID_RemoveUser);
  CommandCenter->RegisterProcess(new DisplayProcess(), CID_Display);

  host.Host();
  // create hosting server
  // if creation fails, let the user know and quit.
  std::cout << "ChatServer2.0\nHosting from: " << NetAPI->LocalIP() << " Port: " << configuration.port_ << std::endl;
  std::cout << "Close this window to exit.\n";

  // while the server hasn't posted a quit message update all ports.
  std::string cmd;
  while (cmd != "server:quit")
  {
    std::cin >> cmd; // read in a command from the user
    // TODO: kick isn't implemented at the moment.
  }
  host.Quit();
  NetAPI->Cleanup();
  return 0;
}
