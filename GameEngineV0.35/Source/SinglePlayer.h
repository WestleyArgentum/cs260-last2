#pragma once

#include "IGameState.h"
#include "Engine.h"
#include "IController.h"
#include "ObjectLinkedList.h"

namespace Framework
{
  class SinglePlayer : public IGameState
  {
    ObjectLinkList<Controller> Controllers;

  public:
    SinglePlayer( GameStateManager *gsm );
    virtual ~SinglePlayer( void );

    virtual void SendMessage( Message *m );

    ///Make asteroids, player, and a score object.
    virtual void Initialize( void );

    ///Update the game, updates all systems.
    virtual void Update( float dt );

    ///Cleanup assets and cleanup and ties to other states.
    virtual void Cleanup( void );

    ///Reset all the positions, objects, scores, ect...
    virtual void Restart( void );

    GOC * CreateObjectAt( Vec2& position, float rotation, const std::string& file) ;

    void LoadLevelFile( const std::string &filename );
    
	  void SpawnRandomAsteroids( void );
  };
}
