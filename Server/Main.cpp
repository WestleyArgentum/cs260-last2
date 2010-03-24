#include "WindowsLibrary/CommandCenter.hpp"
#include "NetworkingLibrary/NetAPI.h"

int main(int argc, char *argv[])
{
  using namespace NAPI;
  // initialize NetAPI
  NetAPI->Init();

  // read in port from config file
  unsigned port = 8000; // hard coded for now. TODO: FIX THIS!!!

  // create hosting server
  // if creation fails, let the user know and quit.
  std::cout << "ChatServer2.0\nHosting from: " << NetAPI->LocalIP() << " Port: " << port << std::endl;
  std::cout << "You can enter commands to control the server here.\nCommands:\n"
               "server:quit .......... Quits the server, disconnecting all users.\n"
               "server:kick <user> ... Kicks the user specified from the server.\n";
  // create client list

  // while the server hasn't posted a quit message update all ports.
  std::string cmd;
  while (cmd != "server:quit")
  {
    std::cin >> cmd; // read in a command from the user
    // TODO: kick isn't implemented at the moment.
  }

  NetAPI->Cleanup();
  return 0;
}
