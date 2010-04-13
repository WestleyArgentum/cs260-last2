#pragma once

#include "IController.h"

namespace Framework
{
  class ScoreDisplay : public Controller
  {
    public:
      ScoreDisplay( void );
		  void LogicalUpdate ( float dt );

    private:
      struct ScoreInfo
      {
        ScoreInfo( void ) : playerID_(0), score_(0) {;}

        unsigned playerID_;
        unsigned score_;
      };    // ScoreInfo

      typedef std::vector<ScoreInfo> ScoreCont;
      ScoreCont scores_;
  };    // ScoreDisplay

}   // namespace Framework

