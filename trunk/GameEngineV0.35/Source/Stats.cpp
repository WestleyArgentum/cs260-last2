#include "Precompiled.h"
#include "Stats.h"

namespace Framework
{
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// PlayerStats Methods

/**************************************************************************************************/
/**************************************************************************************************/
  int PlayerStats::SerializeData( char *buffer ) const
  {
    return sizeof(PlayerStats);
  }

/**************************************************************************************************/
/**************************************************************************************************/
  int PlayerStats::InterpretData( char *buffer )
  {
    return sizeof(PlayerStats);
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// UpdateStats Methods

/**************************************************************************************************/
/**************************************************************************************************/
  unsigned UpdateStats::Size( void ) const
  {
      // Size of the vector that we are passing across the network, then the size of the array that
      //  we need to send.
    return sizeof(unsigned) + sizeof(PlayerStats) * stats_.size();
  }

/**************************************************************************************************/
/**************************************************************************************************/
  int UpdateStats::SerializeData( char *buffer, unsigned /*size*/ ) const
  {
      // Store the number of elements within our array into the given buffer.
    *reinterpret_cast<unsigned*>(buffer) = stats_.size();
    buffer += sizeof(unsigned);

      // Store all the PlayerStats within the given buffer too!
    for ( Statistics::const_iterator it = stats_.begin(); it != stats_.end(); ++it )
    {
      buffer += it->SerializeData( buffer );
    }

    return Size();
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void UpdateStats::InterpretData( char *buffer, unsigned /*size */ )
  {
      // Pull out the size of the array stored in the buffer 
    unsigned size = *reinterpret_cast<unsigned*>(buffer);
    buffer += sizeof(unsigned);

    stats_.reserve( size );

    PlayerStats playerStats;

    for ( unsigned i = 0; i < size; ++i )
    {
      buffer += playerStats.InterpretData( buffer );

      stats_.push_back( playerStats );
    }
  }

}   // namespace Framework

