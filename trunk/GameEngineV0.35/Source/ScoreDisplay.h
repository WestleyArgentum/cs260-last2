#pragma once

#include "IController.h"
#include "Stats.h"
#include "Text.h"

namespace Framework
{
    /// Displays the ScoreList that gets sent to this object by message
  class DisplayScore : public Controller
  {
    public:
      DisplayScore( void ) : Controller(CID_DisplayScore) {;}
      ~DisplayScore( void ) {;}

      void SetColor( Color color );
      inline void SetColor( Vec4 color ) { SetColor( Convert(color) ); }

    private:
      void Serialize( ISerializer &stream );
		  void OnInitialize( void );
		  void LogicalUpdate ( float dt );

    private:
      int amount_;          ///< Number to display
      float time_;          ///< Time to display this text before removing it
      float speed_;         ///< Speed at which to move this text in the direction desired.
      Vec2 dir_;            ///< The direction to move this text in.

      Color playerColor_;   ///< The color of the player that spawned this text.

      float maxTime_;
  };    // DisplayScore

    /// Displays the ScoreList that gets sent to this object by message
  class ScoreHUD : public Controller
  {
    public:
      ScoreHUD( void );

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
  };    // ScoreHUD

}   // namespace Framework

