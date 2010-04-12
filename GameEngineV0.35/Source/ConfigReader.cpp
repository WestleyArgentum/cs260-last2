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

#include "Precompiled.h"
#include "ConfigReader.hpp"

#include <fstream>    // std::ifstream
#include <algorithm>  // std::transform
#include <stdio.h>

/**************************************************************************************************/
/**************************************************************************************************/
Config::Config( const char *configname )
{
    // Attempt to open the file
  std::ifstream iStream( configname );

  if ( iStream.is_open() )
  {
    std::string dataname;
    iStream >> dataname;

    while ( iStream.good() )
    {
      std::transform( dataname.begin(), dataname.end(), dataname.begin(), toupper );

      if ( dataname == "[USERNAME]" )     // Specifing username.
      {
        std::string name;

          // Ignore the newline character still left in the stream before the next line of our file
          //  so we can read in the user's name's line.
        iStream.ignore();
        std::getline(iStream, name);
        //iStream.getline( name, sizeof(name) - 1 );
        unsigned len = name.size();
        username_.assign( name.c_str(), (len > 31 ? 31 : len) );
        //username_.push_back('\0');
      }
      else if ( dataname == "[SERVER]" )  // Specifing server information.
      {
        iStream >> dataname;

        while ( iStream.good() )
        {
          std::transform( dataname.begin(), dataname.end(), dataname.begin(), toupper );

          if ( dataname == "IP:" )        // Specifing server ip address.
          {
            iStream >> ip_;
          }
          else if ( dataname == "PORT:" ) // Server port.
          {
            iStream >> port_;
          }
          else
          {
            break;
          }

          iStream >> dataname;
        }

          // Prevent reading in the next 'dataname' within our stream since we read in "pre-read"
          //  the 'dataname' earlier.
        continue;
      }
      else if ( dataname == "[UDP_RANGE]" )
      {
        iStream >> dataname;

        while ( iStream.good() )
        {
          std::transform( dataname.begin(), dataname.end(), dataname.begin(), toupper );

          if ( dataname == "HIGH:" )        // Specifing server ip address.
          {
            iStream >> range_.high_;
          }
          else if ( dataname == "LOW:" ) // Server port.
          {
            iStream >> range_.low_;
          }
          else
          {
            break;
          }

          iStream >> dataname;
        }

        continue;
      }

      iStream >> dataname;
    }
  }
  else
  {
      // Config file not found!
    throw std::logic_error( "Error: Config file [" + std::string( configname ) + "] not found." );
  }
}

