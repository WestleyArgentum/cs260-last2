#pragma once

#include "Message.h"
#include "INetMessage.h"

namespace Framework
{
  struct PlayerStats
  {
    PlayerStats( void ) : playerId_(0), score_(0) {;}
    PlayerStats( unsigned playerId, unsigned score ) : playerId_(playerId), score_(score) {;}

    int SerializeData( DataStream &stream ) const;
    int InterpretData( DataStream &stream );

    unsigned playerId_;
    unsigned score_;
  };    // PlayerStats

  typedef std::vector<PlayerStats> Statistics;

  class UpdateStats : public Message, INetMessage
  {
    public:
      UpdateStats( void ) : Message(Mid::StatsUpdate) {;}
      explicit UpdateStats( const Statistics &stats ) : Message(Mid::StatsUpdate), stats_(stats) {;}


      ///Returns the type of message this is.
      virtual Mid::MessageIdType Type( void ) const { return MessageId; }

      ///Creates a carbon copy of the message.
      virtual UpdateStats * Clone( void ) const { return new UpdateStats(*this); }

        /// Return the size of the information that we need to write to the given buffer.
      virtual unsigned Size( void ) const;

        /// Write the statistics of this structure to the the buffer given.
      virtual int SerializeData( DataStream &stream ) const;

        /// Build the statistics structure getting the data from the buffer given!
      virtual void InterpretData( DataStream &stream );

    public:
      Statistics stats_;    ///< Copy of the current statistics of our game.
  };    // UpdateStats

}   // namespace Framework

