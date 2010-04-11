
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
			body->AddForce(Vec2(cos(transform->Rotation - 90) * speed, sin(transform->Rotation - 90) * speed));
		if( IsDownHeld() )
			body->AddForce(-Vec2(cos(transform->Rotation) * speed, sin(transform->Rotation) * speed));
		if( IsLeftHeld() )
			transform->Rotation += .09;  // hard coded rotate by rads
		if( IsRightHeld() )
			transform->Rotation -= .09; // hard coded rotate by rads
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

	void PlayerController::Serialize(ISerializer& stream)
	{
		StreamRead(stream, speed);
	}
}
