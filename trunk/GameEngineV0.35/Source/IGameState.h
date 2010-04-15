#pragma once

#include "Message.h"
#include "IController.h"

namespace Framework
{
  ///Forward declaration for pointer reasons.
  class GameStateManager;

  ///The GameState interface, used to update gameplay and menu screens.
  class IGameState
  {
  protected:
    GameStateManager *gsm_;

  public:
    IGameState( GameStateManager *gsm ) : gsm_(gsm) {;}
    ///Make sure all derived destructors are virtual.
    virtual ~IGameState( void ) {;}

    ///Adds a controller to the internal list of the GameState.
    virtual void AddController( Controller *controller ) = 0;

    ///Removes a controller from the internal list of the GameState.
    virtual void RemoveController( Controller *controller ) = 0;

    ///Standard send message function.
    virtual void SendMessage( Message *m ) = 0;

    ///Initialize the GameState, load any assets and object.
    virtual void Initialize( void ) = 0;

    ///Update the GameState, run logic, update systems.
    virtual void Update( float dt ) = 0;

    ///Cleanup and allocated data, assets, and objects.
    virtual void Cleanup( void ) = 0;

    ///Restart the GameState, reinit objects and add anything missing.
    virtual void Restart( void ) = 0;
  };
}
