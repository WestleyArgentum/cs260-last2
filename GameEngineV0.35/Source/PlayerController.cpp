
// PlayerController.cpp : The manifestation (in code) of the brains of the character.

#include "Precompiled.h"
#include "PlayerController.h"
#include "WindowsSystem.h"

namespace Framework
{

	 // shouldn't be here
	#define PI 3.14159265358979323846f
	#define DEG_TO_RAD (PI / 180.0f)

	PlayerController::PlayerController()
	{
		// could set default value for character speed, etc... but who would forget to set those hahaha
	}

	PlayerController::~PlayerController()
	{
		// if you add something to the character destroy it here
	}

	void PlayerController::LogicalUpdate( float dt )
	{
		if( IsUpHeld() )
			body->AddForce(Vec2(cos(transform->Rotation - 89.5f) * speed, sin(transform->Rotation - 89.5f) * speed));  //^! <-- odd
		if( IsDownHeld() )
			body->AddForce(-Vec2(cos(transform->Rotation - 89.5f) * speed, sin(transform->Rotation - 89.5f) * speed));
		if( IsLeftHeld() )
			transform->Rotation += rot_angle * DEG_TO_RAD;
		if( IsRightHeld() )
			transform->Rotation -= rot_angle * DEG_TO_RAD;
	}

	void PlayerController::DestroyCheck()
	{
		if(health <= 0)
		{
			FACTORY->Destroy(GetOwner());
			//^! here we should also end the match (for this player) and maybe take them to a lose screen
		}

		// Here we could kill the character if he flew out of bounds or something also
	}

	void PlayerController::Serialize(ISerializer& stream)
	{
		StreamRead(stream, health);
		StreamRead(stream, speed);
		StreamRead(stream, rot_angle);
	}
}
