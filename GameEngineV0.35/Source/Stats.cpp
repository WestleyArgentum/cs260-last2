#include "Precompiled.h"
#include "Stats.h"
#include "GameStateManager.h"
#include "MessageHub.h"

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
    StreamRead( stream, playerId_ );
    StreamRead( stream, score_ );
    StreamRead( stream, color_ );

    return sizeof(PlayerStats);
  }

/**************************************************************************************************/
/**************************************************************************************************/
  int StatsFind( const Statistics &stats, unsigned playerId )
  {
    for ( unsigned i = 0; i < stats.size(); ++i )
    {
      if ( stats[i].playerId_ == playerId )
      {
        return i;
      }
    }

    return -1;
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// StatsMessage Methods

/**************************************************************************************************/
/**************************************************************************************************/
  //unsigned StatsMessage::Size( void ) const
  //{
  //    // Size of the vector that we are passing across the network, then the size of the array that
  //    //  we need to send.
  //  return sizeof(unsigned) + sizeof(PlayerStats) * stats_.size();
  //}

/**************************************************************************************************/
/**************************************************************************************************/
  int StatsMessage::SerializeData( DataStream &stream ) const
  {
      // Store the number of elements within our array into the given buffer.
    stream.WriteData(stats_.size());

      // Store all the PlayerStats within the given buffer too!
    for ( Statistics::const_iterator it = stats_.begin(); it != stats_.end(); ++it )
    {
      it->SerializeData( stream );
    }

    return stream.Size();
  }

/**************************************************************************************************/
/**************************************************************************************************/
  void StatsMessage::InterpretData( DataStream &stream )
  {
      // Pull out the size of the array stored in the buffer 
    unsigned size = 0;
    stream.ReadData(size);

    stats_.reserve( size );

    PlayerStats playerStats;

    for ( unsigned i = 0; i < size; ++i )
    {
      stats_.push_back( playerStats );

      stats_[i].InterpretData( stream );
    }
  }

  void StatsMessage::SendThis( void )
  {
    MessageHub->Post( StatsMessage(stats_) );
    GSM->SendMessage(this);
  }

}   // namespace Framework

