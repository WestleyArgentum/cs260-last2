#pragma once

namespace Framework
{
  ///The GameState interface, used to update gameplay and menu screens.
  class IGameState
  {
  public:
    ///Make sure all derived destructors are virtual.
    virtual ~IGameState( void ) {;}

    ///Initialize the GameState, load any assets and object.
    virtual void Initialize( void ) = 0;

    ///Update the GameState, run logic, update systems.
    virtual void Update( void ) = 0;

    ///Cleanup and allocated data, assets, and objects.
    virtual void Cleanup( void ) = 0;

    ///Restart the GameState, reinit objects and add anything missing.
    virtual void Restart( void ) = 0;
  };
}
