
// PlayerController.cpp : The manifestation (in code) of the brains of the character.

#include "Precompiled.h"
#include "PlayerController.h"
#include "WindowsSystem.h"

namespace Framework
{
	PlayerController::PlayerController() : health (3) //^! hard-coded default health
	{
		// could set default value for character speed, etc...
	}

	PlayerController::~PlayerController()
	{
		// if you add something to the character destroy it here
	}

	void PlayerController::LogicalUpdate( float dt )
	{
		if( IsUpHeld() )
			body->AddForce(Vec2(0, Speed /** dt*/));
		if( IsDownHeld() )
			body->AddForce(Vec2(0, -Speed /** dt*/));
		if( IsLeftHeld() )
			body->AddForce(Vec2(-Speed /** dt*/, 0));
		if( IsRightHeld() )
			body->AddForce(Vec2(Speed /** dt*/, 0));
	}

	void PlayerController::DestroyCheck()
	{
		if(health < 0)
		{
			FACTORY->Destroy(GetOwner());
			//^! here we should also end the match (for this player) and maybe take them to a lose screen
		}

		// Here we could kill the character if he flew out of bounds or something also
	}
}
