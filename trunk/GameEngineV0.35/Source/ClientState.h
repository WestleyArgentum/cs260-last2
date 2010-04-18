#pragma once

#include "IGameState.h"

namespace Framework
{

	// The Client will send input to the server and receive messages detailing
	// the goings on of the game.
	class ClientState : public IGameState
	{
    void HandleCreate( INetMessage *msg );
    void HandleDestroy( INetMessage *msg );
    void HandlePlayerId( INetMessage *msg );

    GOCId playerid_;
	public:
		// Create, init,  and destroy methods -----
		ClientState( GameStateManager *gsm );
		virtual ~ClientState( void );

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
	};
}