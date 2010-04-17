#pragma once

#include "IGameState.h"
#include "Engine.h"
#include "IController.h"
#include "ObjectLinkedList.h"

namespace Framework
{
  class SinglePlayer : public IGameState
  {
	private:
    ObjectLinkList<Controller> Controllers;

  public:
    SinglePlayer( GameStateManager *gsm );
    virtual ~SinglePlayer( void );

    ///Adds a controller to the internal list of the GameState.
    virtual void AddController( Controller *controller );

    ///Removes a controller from the internal list of the GameState.
    virtual void RemoveController( Controller *controller );

    virtual void SendMessage( Message *m );

    ///Make asteroids, player, and a score object.
    virtual void Initialize( void );

    ///Update the game, updates all systems.
    virtual void Update( float dt );

    ///Cleanup assets and cleanup and ties to other states.
    void OnCleanup( void );

    ///Reset all the positions, objects, scores, ect...
    virtual void Restart( void );

		GOCId player_ship_id;

	private:
		virtual void LoadFromFile ( const std::string &filename );
  };
}
