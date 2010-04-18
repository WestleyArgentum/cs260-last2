#pragma once

#include "IController.h"
#include "Stats.h"
#include "Text.h"

namespace Framework
{
    /// Displays the ScoreList that gets sent to this object by message
  class ScoreDisplay : public Controller
  {
    public:
      ScoreDisplay( void );

    private:
      void Serialize( ISerializer &stream );
		  void OnInitialize( void );
		  void LogicalUpdate ( float dt );

  		void SendMessage( Message *message );

    private:
      Statistics stats_;
      Text playerText_;
      Transform playerTransform_;

      typedef std::vector<Text>       TextComps;
      typedef std::vector<Transform>  TransformComps;
        /// Will display the score of each player using different text objects so we can show the
        ///  text with different colors and different sizes.
      TextComps text_;
      TransformComps transforms_;
  };    // ScoreDisplay

}   // namespace Framework

