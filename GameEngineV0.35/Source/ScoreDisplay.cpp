#include "Precompiled.h"
#include "ScoreDisplay.h"

#include "Text.h"

namespace Framework
{
  ScoreDisplay::ScoreDisplay( void ) : scores_(8)
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
}   // namespace Framework

