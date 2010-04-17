
#include "Precompiled.h"
#include "ServerState.h"

namespace Framework
{

	Framework::ServerState::ServerState( GameStateManager *gsm ) : IGameState( gsm )
 	{
 
 	}

	Framework::ServerState::~ServerState( void )
	{

	}

	void Framework::ServerState::Initialize( void )
	{
		LoadFromFile("Levels\\Server.txt");
	}

	void Framework::ServerState::OnCleanup( void )
	{

	}

	void Framework::ServerState::AddController( Controller *controller )
	{

	}

	void Framework::ServerState::RemoveController( Controller *controller )
	{

	}

	void Framework::ServerState::SendMessage( Message *m )
	{

	}

	void Framework::ServerState::Update( float dt )
	{

	}

	void Framework::ServerState::Restart( void )
	{

	}

}
