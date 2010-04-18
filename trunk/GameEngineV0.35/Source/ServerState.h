#pragma once

#include "IGameState.h"

namespace Framework
{

	// The Server will be running the simulation, receiving input from clients,
	// and posting messages about created, destroyed, and updated objects.
	class ServerState : public IGameState
  {
    void HandleConnection(INetMessage *msg);
    void HandleCreate(INetMessage *msg);
    void HandleDestroy(INetMessage *msg);
    void HandleInput(INetMessage *msg);

	public:
		// Create, init,  and destroy methods -----
		ServerState( GameStateManager *gsm );
		virtual ~ServerState( void );

		virtual void Initialize( void );
		virtual void OnCleanup( void );
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

	private:
		ObjectLinkList<Controller> Controllers;
	};
}
