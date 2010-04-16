#pragma once

#include "IGameState.h"
#include "Engine.h"
#include "IController.h"

namespace Framework
{
  class MainMenu : public IGameState
  {
    public:
      MainMenu( GameStateManager *gsm );
      virtual ~MainMenu( void );

      ///Adds a controller to the internal list of the GameState.
      virtual void AddController( Controller *controller );

      ///Removes a controller from the internal list of the GameState.
      virtual void RemoveController( Controller *controller );

      virtual void SendMessage( Message *message );

      ///Make asteroids, player, and a score object.
      virtual void Initialize( void );

      ///Update the game, updates all systems.
      virtual void Update( float dt );

      ///Cleanup assets and cleanup and ties to other states.
      virtual void OnCleanup( void );

      ///Reset all the positions, objects, scores, ect...
      virtual void Restart( void );

    private:
      typedef ObjectLinkList<Controller> ControllerList;
      ControllerList Controllers;
  };    // MainMenu

}   // namespace Framework

