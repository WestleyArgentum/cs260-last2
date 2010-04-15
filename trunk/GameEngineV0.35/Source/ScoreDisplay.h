#pragma once

#include "IController.h"
#include "Stats.h"

namespace Framework
{
    /// Displays the ScoreList that gets sent to this object by message
  class ScoreDisplay : public Controller
  {
    public:
      ScoreDisplay( void );
		  void LogicalUpdate ( float dt );

  		void SendMessage( Message *message );

    private:
      Statistics stats_;
  };    // ScoreDisplay

}   // namespace Framework

