
// IController.cpp : "Brains" or "Logic component" for entities. Updated by logic every loop.

#include "Precompiled.h"
#include "IController.h"
#include "GameLogic.h"

namespace Framework
{

	Controller::Controller()
	{
		//Set the default speed
		Speed = 50.0f;
	}

	Controller::~Controller()
	{
		LOGIC->Controllers.erase( this );
	}

	void Controller::Initialize()
	{ 
		transform = GetOwner()->has(Transform);
		body = GetOwner()->has(Body);
		LOGIC->Controllers.push_back( this );
	}

	void Controller::Update(float dt)
	{
		LogicalUpdate(dt);
		DestroyCheck();
	}

	void Controller::Serialize(ISerializer& stream)
	{
		StreamRead(stream,Speed);
	}

	void Controller::DestroyCheck()
	{
		//^! destroy things if the get too far from the player or the center or something
	}
}
