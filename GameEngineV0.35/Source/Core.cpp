///////////////////////////////////////////////////////////////////////////////////////
//
//	Core.cpp
//	Authors:  Benjamin Ellinger, Chris Peters
//	Copyright 2009, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "Core.h"

#include "Timer.h"
#include "PRNG.h"

namespace Framework
{
	//A global pointer to the core
	CoreEngine* CORE;

  CoreEngine::CoreEngine() : LastTime(0), TotalTime(0.0), GameActive(true)
	{
		CORE = this; //Set the global pointer
	}

	CoreEngine::~CoreEngine()
	{
		//Nothing for the destructor to do
	}

	void CoreEngine::Initialize()
	{
		// seed the random generator
		Utils::srand(GetTickCount(), timeGetTime());

		for (unsigned i = 0; i < Systems.size(); ++i)
			Systems[i]->Initialize();
	}

	void CoreEngine::GameLoop()
	{
		//Initialize the last time variable so our first frame
		//is "zero" seconds (and not some huge unknown number)
    Timer frametimer;

    LastTime = frametimer.TimeElapsed();

		while (GameActive)
		{
			//Get the current time in milliseconds
			double currenttime = frametimer.TimeElapsed();
			//Convert it to the time passed since the last frame (in seconds)
			double dt = (currenttime - LastTime);
			//Update the when the last update started
			LastTime = currenttime;
      TotalTime += dt;

			//Update every system and tell each one how much
			//time has passed since the last update
			for (unsigned i = 0; i < Systems.size(); ++i)
				Systems[i]->Update( static_cast<float>(dt) );
		}

	}

	void CoreEngine::BroadcastMessage(Message* message)
	{
		//The message that tells the game to quit
		if (message->MessageId == Mid::Quit)
			GameActive = false;

		//Send the message to every system--each
		//system can figure out whether it cares
		//about a given message or not
		for (unsigned i = 0; i < Systems.size(); ++i)
			Systems[i]->SendMessage(message);
	}

	void CoreEngine::AddSystem(ISystem* system)
	{
		//Add a system to the core to be updated
		//every frame
		Systems.push_back(system);
	}

  ///Adds a system currently on the systems vec to the inactive systems vec
  void CoreEngine::SleepSystem(ISystem* system)
  {
    // find the system
    std::vector<ISystem*>::iterator sys = std::find(Systems.begin(), Systems.end(), system);

    // add the system to the new vec (provided we found it)
    if(sys != Systems.end())
    {
      InactiveSystems.push_back(*sys);
      Systems.erase(sys);
    }
  }

	void CoreEngine::DestroySystems()
	{			
		//Delete all the systems in reverse order
		//that they were added in (to minimize any
		//dependency problems between systems)
		for (unsigned i = 0; i < Systems.size(); ++i)
		{
			delete Systems[Systems.size()-i-1];
		}

    //Well, at least in the relative order they were added.
    //Hopefully if they are sleeping there wont be any
    //issues anyway.
    for (unsigned i = 0; i < InactiveSystems.size(); ++i)
		{
			delete InactiveSystems[Systems.size()-i-1];
		}
	}
}
