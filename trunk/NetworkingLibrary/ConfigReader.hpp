#pragma once

#include <string>     // std::string

  /// Reads in the config information
struct Config
{
  explicit Config( const char *configname );

  std::string username_;  ///< Client's user name displayed for chat purposes.

  std::string ip_;        ///< Server IP address.
  unsigned short port_;   ///< Server Port address.
};

