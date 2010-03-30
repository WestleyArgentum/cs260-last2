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

