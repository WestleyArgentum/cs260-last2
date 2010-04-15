
// IController.cpp : "Brains" or "Logic component" for entities. Updated by logic every loop.

#include "Precompiled.h"
#include "IController.h"
#include "GameStateManager.h"

namespace Framework
{

	Controller::Controller()
  {
    GSM->AddController( this );
  }

	Controller::~Controller()
	{
		GSM->RemoveController( this );
	}

  void Controller::Initialize( void )
  {
  }

	void Controller::Update(float dt)
	{
		LogicalUpdate(dt);
		DestroyCheck();
	}

	void Controller::Serialize(ISerializer& stream)
	{}

	void Controller::DestroyCheck()
	{
		//^! destroy things if the get too far from the player or the center or something
	}

}
