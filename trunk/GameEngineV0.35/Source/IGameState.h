#pragma once

#include <algorithm>

#include "Message.h"
#include "IController.h"
#include "Factory.h"
#include "UtilityGameFunctions.h"
#include "TextSerialization.h"

namespace Framework
{
  ///Forward declaration for pointer reasons.
  class GameStateManager;

  ///The GameState interface, used to update gameplay and menu screens.
  class IGameState
  {
  protected:
    GameStateManager *gsm_;

		///Load from a file
		virtual void LoadFromFile( const std::string &filename )
		{
			TextSerializer stream;
			bool fileOpened = stream.Open(filename);
			ErrorIf( !fileOpened , "Could not open file %s. File does not exist or is protected." , filename.c_str() );

			std::string objectArchetype;
			Vec2 objectPosition;
			float objectRotation;

			while(stream.IsGood())
			{
				StreamRead(stream,objectArchetype);
				StreamRead(stream,objectPosition);
				StreamRead(stream,objectRotation);
				if (stream.IsGood())
					CreateObjectAt(objectPosition, objectRotation, objectArchetype);
			}
		}

		virtual void OnCleanup( void ) = 0;

		///Vec of Game object Id's that belong to this state
		typedef std::vector<GOCId> GameObjectIds;
		GameObjectIds MyGameObjects;

  public:
    IGameState( GameStateManager *gsm ) : gsm_(gsm) { MyGameObjects.reserve(1000); }
    ///Make sure all derived destructors are virtual.
    virtual ~IGameState( void ) {;}

    ///Adds a controller to the internal list of the GameState.
    virtual void AddController( Controller *controller ) = 0;

    ///Removes a controller from the internal list of the GameState.
    virtual void RemoveController( Controller *controller ) = 0;

		void RegisterComposition (GOCId id)
		{
			MyGameObjects.push_back(id);
		}

    ///Standard send message function.
    virtual void SendMessage( Message *m ) = 0;

    ///Initialize the GameState, load any assets and object.
    virtual void Initialize( void ) = 0;

    ///Update the GameState, run logic, update systems.
    virtual void Update( float dt ) = 0;

    ///Cleanup and allocated data, assets, and objects.
    void Cleanup( void )
		{
			//for_each(MyGameObjects.begin(), MyGameObjects.end(), std::bind1st(std::mem_fun_ref(&GameObjectFactory::DestroyById), *FACTORY));
			for(GameObjectIds::iterator it = MyGameObjects.begin(); it != MyGameObjects.end(); ++it)
			{
				FACTORY->DestroyById(*it);
			}
		}

    ///Restart the GameState, reinit objects and add anything missing.
    virtual void Restart( void ) = 0;
  };
}

