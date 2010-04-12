
// PlayerController.cpp : The manifestation (in code) of the brains of the character.

#include "Precompiled.h"

#include "PlayerController.h"
#include "WindowsSystem.h"
#include "GameLogic.h"
#include "MessageHub.h"

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

	void PlayerController::Initialize()
	{
		transform = GetOwner()->has(Transform);
		body = GetOwner()->has(Body);
		LOGIC->Controllers.push_back( this );

		MessageHub->Register(GetOwner()->GetId(), Mid::MouseButton);
	}

	void PlayerController::LogicalUpdate( float dt )
	{
		// handle movement
		if( IsUpHeld() )
			body->AddForce(Vec2(cos(transform->Rotation - 89.5f) * speed, sin(transform->Rotation - 89.5f) * speed));  //^! <-- odd
		if( IsDownHeld() )
			body->AddForce(-Vec2(cos(transform->Rotation - 89.5f) * speed, sin(transform->Rotation - 89.5f) * speed));
		if( IsLeftHeld() )
			transform->Rotation += rot_angle * DEG_TO_RAD;
		if( IsRightHeld() )
			transform->Rotation -= rot_angle * DEG_TO_RAD;

		if( GOC * grabbedObject = FACTORY->GetObjectWithId(GrabbedObjectId))
		{
			Body * body = grabbedObject->has(Body);
			if( IsShiftHeld() )
			{
				//Hard set or Teleport the object
				body->SetPosition(LOGIC->WorldMousePosition);
				body->SetVelocity(Vec2(0,0));
			}
			else
			{
				//Shove the object around
				body->AddForce( (LOGIC->WorldMousePosition - body->Position) * 50 );
			}
		}
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

	void PlayerController::SendMessage( Message * message )
	{
		switch( message->MessageId )
		{
		case Mid::MouseButton:
			{
				MouseButton * mouse = static_cast<MouseButton*>(message);
				//Update the world mouse position
				LOGIC->WorldMousePosition = GRAPHICS->ScreenToWorldSpace(mouse->MousePosition);

				if(mouse->ButtonIsPressed)
				{
					if( mouse->MouseButtonIndex == MouseButton::LeftMouse )
					{
						//On left click attempt to grad a object at the mouse cursor
						GOC * goc = PHYSICS->TestPoint( LOGIC->WorldMousePosition );
						if( goc )
							GrabbedObjectId = goc->GetId();
						else
							return;

						// Give the asteroid a new color
						Sprite* sprite = goc->has(Sprite);
						if (sprite)
							sprite->Color = GetOwner()->has(Sprite)->Color;
					}
				}
				else
				{
					//If the mouse has been release let go of the grabbed object
					GOC* asteroid = FACTORY->GetObjectWithId(GrabbedObjectId);
					if (asteroid)
					{
						Sprite* sprite = asteroid->has(Sprite);
						if (sprite)
							sprite->Color = Vec4(1.0f, 1.0f, 0, 1.0f);  //^! hardcoded!
					}

					GrabbedObjectId = 0;
				}
				break;
			}
		}
	}

}
