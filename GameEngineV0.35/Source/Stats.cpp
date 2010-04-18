#include "Precompiled.h"
#include "Stats.h"

namespace Framework
{
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// PlayerStats Methods

/**************************************************************************************************/
/**************************************************************************************************/
  int PlayerStats::SerializeData( DataStream &stream ) const
  {
    StreamWrite( stream, playerId_ );
    StreamWrite( stream, score_ );
    StreamWrite( stream, color_ );

    return sizeof(PlayerStats);
  }

/**************************************************************************************************/
/**************************************************************************************************/
  int PlayerStats::InterpretData( DataStream &stream )
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
  int UpdateStats::SerializeData( DataStream &stream ) const
  {
      // Store the number of elements within our array into the given buffer.
    stream.WriteUInt(stats_.size());

      // Store all the PlayerStats within the given buffer too!
    for ( Statistics::const_iterator it = stats_.begin(); it != stats_.end(); ++it )
    {
      StreamWrite( stream, *it );
    }

    return Size();
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void UpdateStats::InterpretData( DataStream &stream )
  {
      // Pull out the size of the array stored in the buffer 
    unsigned size = 0;
    stream.ReadUInt(size);

    stats_.reserve( size );

    PlayerStats playerStats;

    for ( unsigned i = 0; i < size; ++i )
    {
      StreamRead( stream, playerStats );

      stats_.push_back( playerStats );
    }
  }

}   // namespace Framework
