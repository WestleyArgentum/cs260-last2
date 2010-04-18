#include "Precompiled.h"
#include "ClientState.h"
#include "Network.h"

Framework::ClientState::ClientState( GameStateManager *gsm ) : IGameState(gsm)
{}

Framework::ClientState::~ClientState( void )
{}

void Framework::ClientState::Initialize( void )
{
	// connect to a server
	ErrorIf(!NETWORK);
	NETWORK->FindServer();
}

void Framework::ClientState::OnCleanup( void )
{}

void Framework::ClientState::AddController( Controller *controller )
{
	// hahaha, no.
}

void Framework::ClientState::RemoveController( Controller *controller )
{
	// ya... na...
}

void Framework::ClientState::SendMessage( Message *m )
{}

void Framework::ClientState::Update( float dt )
{
}

void Framework::ClientState::Restart( void )
{}