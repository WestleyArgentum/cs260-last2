#include "Precompiled.h"
#include "ScoreDisplay.h"

#include "MessageHub.h"
#include "Graphics.h"
#include "GameStateManager.h"

const unsigned MAX_PLAYERS = 8;

namespace Framework
{
  namespace
  {
    struct HighestScore : public std::binary_function< const PlayerStats&, const PlayerStats&, bool>
    {
      bool operator()( const PlayerStats &lhs, const PlayerStats &rhs ) const
      {
        return lhs.score_ > rhs.score_;
      }
    };    // HighestScore
  }   // namespace

  ScoreDisplay::ScoreDisplay( void ) : transforms_(MAX_PLAYERS - 1)
  {
    text_.reserve(MAX_PLAYERS - 1);
  }

  void ScoreDisplay::Serialize( ISerializer &stream )
  {
    unsigned width, height;
    StreamRead( stream, height );
    StreamRead( stream, width );

    StreamRead( stream, playerText_ );

      // Initialize the player's score text
    GRAPHICS->TextList.push_back( &playerText_ );
    playerText_.transform = &playerTransform_;

    for ( unsigned i = 0; i < MAX_PLAYERS - 1; ++i )
    {
        // Make a copy of this player text
      text_.push_back( playerText_ );

        // Self initalize this text object to get drawn using the transform components we gave it
        //  similuating that it was attached to a object.
      GRAPHICS->TextList.push_back( &text_[i] );
      text_[i].transform = &transforms_[i];
    }

    playerText_.ChangeFont( width, height, playerText_.fontname_ );
  }

  void ScoreDisplay::OnInitialize( void )
  {
    MessageHub->Register( GetOwner()->GetId(), Mid::StatsUpdate );
  }

  void ScoreDisplay::LogicalUpdate( float dt )
  {
      // Sort the statistics by the highest score, highest score on top.
    std::sort( stats_.begin(), stats_.end(), HighestScore() );

    Vec2 start( 5, 0 );

    Statistics::iterator it = stats_.begin();
    for ( unsigned i = 0; i < MAX_PLAYERS && it != stats_.end(); ++it )
    {
      if ( it->playerId_ == GSM->GetPlayerId() )    // If we are draw the current player's score
      {
  // 'argument' : conversion from 'FLOAT' to 'unsigned char', possible loss of data
  // Value of the float should be 0 and 255 so the cast will not lose any data.
#pragma warning( push )
#pragma warning( disable : 4244 )
        playerText_.SetColor( it->color_.r, it->color_.g, it->color_.b, it->color_.a );
#pragma warning( pop )

        playerText_.SetText( "Score: %u", it->score_ );

        playerTransform_.Position = start;
        start.y += 22;
      }
      else    // Otherwise we are drawing someone other player's score
      {
        Text &current = text_[i];

  // 'argument' : conversion from 'FLOAT' to 'unsigned char', possible loss of data
  // Value of the float should be 0 and 255 so the cast will not lose any data.
#pragma warning( push )
#pragma warning( disable : 4244 )
        current.SetColor( it->color_.r, it->color_.g, it->color_.b, it->color_.a );
#pragma warning( pop )
        current.SetText( "Score: %u", it->score_ );
 
        current.transform->Position = start;
        start.y += 20;

        ++i;
      }
    }
  }

  void ScoreDisplay::SendMessage( Message *message )
  {
    switch ( message->MessageId )
    {
      case Mid::StatsUpdate:
        {
          UpdateStats *updatestats = static_cast<UpdateStats*>( message );

          stats_ = updatestats->stats_;
        }
        break;
    }
  }

}   // namespace Framework

