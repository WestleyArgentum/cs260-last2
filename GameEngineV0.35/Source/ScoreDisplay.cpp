#include "Precompiled.h"
#include "ScoreDisplay.h"

#include "Text.h"

namespace Framework
{
  ScoreDisplay::ScoreDisplay( void )
  {
  }

  void ScoreDisplay::LogicalUpdate( float dt )
  {
    Text *text = GetOwner()->has(Text);

    if ( text )
    {
        // Temporary, need a way to get player id and score.
      text->SetText( "Player %u\nScore: %10u", 0, 0 );
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

