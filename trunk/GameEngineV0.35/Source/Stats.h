#pragma once

#include "Message.h"
#include "INetMessage.h"

namespace Framework
{
  struct PlayerStats
  {
    PlayerStats( void ) : playerId_(0), score_(0) {;}

    int SerializeData( char *buffer ) const;
    int InterpretData( char *buffer );

    unsigned playerId_;
    unsigned score_;
  };    // PlayerStats

  typedef std::vector<PlayerStats> Statistics;

  class UpdateStats : public Message, INetMessage
  {
    public:
      UpdateStats( void ) : Message(Mid::StatsUpdate) {;}
      explicit UpdateStats( const Statistics &stats ) : Message(Mid::StatsUpdate), stats_(stats) {;}

        /// Return the size of the information that we need to write to the given buffer.
      virtual unsigned Size( void ) const;

        /// Write the statistics of this structure to the the buffer given.
      virtual int SerializeData( char *buffer, unsigned ) const;

        /// Build the statistics structure getting the data from the buffer given!
      virtual void InterpretData( char *buffer, unsigned size );

    public:
      Statistics stats_;    ///< Copy of the current statistics of our game.
  };    // UpdateStats

}   // namespace Framework

