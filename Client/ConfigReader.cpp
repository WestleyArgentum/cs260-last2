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
        char name[32] = {0};

          // Ignore the newline character still left in the stream before the next line of our file
          //  so we can read in the user's name's line.
        iStream.ignore();
        iStream.getline( name, sizeof(name) );

        username_.assign( name, sizeof(name) );
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

      iStream >> dataname;
    }
  }
  else
  {
      // Config file not found!
    throw std::logic_error( "Error: Config file [" + std::string( configname ) + "] not found." );
  }
}

