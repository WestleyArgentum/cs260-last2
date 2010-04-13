#pragma once

#include "IGameState.h"

namespace Framework
{
  class SinglePlayer : public IGameState
  {
  public:
    SinglePlayer( void );
    virtual ~SinglePlayer( void );

    ///Make asteroids, player, and a score object.
    virtual void Initialize( void );

    ///Update the game, updates all systems.
    virtual void Update( void );

    ///Cleanup assets and cleanup and ties to other states.
    virtual void Cleanup( void );

    ///Reset all the positions, objects, scores, ect...
    virtual void Restart( void );
  };
}
