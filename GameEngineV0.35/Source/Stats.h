#pragma once

#include "VMath.h"        // Color
#include "Message.h"
#include "INetMessage.h"

namespace Framework
{
  struct PlayerStats
  {
    PlayerStats( void ) : playerId_(0), score_(0), color_(255, 255, 255, 255) {;}
    PlayerStats( unsigned playerId, unsigned score, Color color = Color(255, 255, 255, 255) )
      : playerId_(playerId), score_(score), color_(color) {;}

    int SerializeData( DataStream &stream ) const;
    int InterpretData( DataStream &stream );

    unsigned playerId_;
    unsigned score_;
    Color color_;
  };    // PlayerStats

  typedef std::vector<PlayerStats> Statistics;

  class StatsMessage : public INetMessage
  {
    public:
      StatsMessage( void ) : INetMessage(Mid::StatsUpdate) {;}
      explicit StatsMessage( const Statistics &stats ) : INetMessage(Mid::StatsUpdate), stats_(stats) {;}

      ///Returns the type of message this is.
      virtual NMid::NetMessageIdType Type( void ) const { return NMid::Stats; }

      ///Creates a carbon copy of the message.
      virtual StatsMessage * Clone( void ) const { return new StatsMessage(*this); }

        /// Return the size of the information that we need to write to the given buffer.
      virtual unsigned Size( void ) const;

        /// Write the statistics of this structure to the the buffer given.
      virtual int SerializeData( DataStream &stream ) const;

        /// Build the statistics structure getting the data from the buffer given!
      virtual void InterpretData( DataStream &stream );

    public:
      Statistics stats_;    ///< Copy of the current statistics of our game.
  };    // StatsMessage

}   // namespace Framework

