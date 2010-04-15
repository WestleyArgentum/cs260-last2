#include "Precompiled.h"
#include "ScoreDisplay.h"
#include "MessageHub.h"
#include "Text.h"

namespace Framework
{
  ScoreDisplay::ScoreDisplay( void )
  {
  }

  void ScoreDisplay::OnInitialize( void )
  {
    MessageHub->Register( GetOwner()->GetId(), Mid::StatsUpdate );
  }

  void ScoreDisplay::LogicalUpdate( float dt )
  {
    Text *text = GetOwner()->has(Text);

    if ( text )
    {
      char buffer[1024] = {0};
      char *walker = buffer;

      for ( Statistics::iterator it = stats_.begin(); it != stats_.end(); ++it )
      {
        walker += sprintf( walker, "Score: %d\n", it->score_ );
      }

      text->SetText( buffer );
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

