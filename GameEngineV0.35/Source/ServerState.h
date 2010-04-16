#pragma once

#include "IGameState.h"

namespace Framework
{

	// The Server will be running the simulation, receiving input from clients,
	// and posting messages about created, destroyed, and updated objects.
	class ServerState : public IGameState
	{
	public:
		// Create, init,  and destroy methods -----
		ServerState( GameStateManager *gsm );
		virtual ~ServerState( void );

		virtual void Initialize( void );
		virtual void Cleanup( void );
		// --------

		// Add / Remove controllers -----
		virtual void AddController( Controller *controller );
		virtual void RemoveController( Controller *controller );
		// --------

		// Receives messages as a system mite, not via message system.
		virtual void SendMessage( Message *m );

		// Update!
		virtual void Update( float dt );

		virtual void Restart( void );
	};
}
