#include "Precompiled.h"
#include "SinglePlayer.h"
#include "PlayerController.h"
#include "Core.h"
#include "GameStateManager.h"
#include "TextSerialization.h"
#include "WindowsSystem.h"
#include "Physics.h"
#include "Asteroid.h"
#include "PRNG.h"
#include <algorithm>

namespace Framework
{
  SinglePlayer::SinglePlayer( GameStateManager *gsm ) : IGameState(gsm)
  {
  }

  SinglePlayer::~SinglePlayer( void )
  {
  }


  void SinglePlayer::SendMessage( Message *m )
  {
    switch( m->MessageId )
    {
      // The user has pressed a (letter/number) key, we may respond by creating
      // a specific object based on the key pressed.
    case Mid::CharacterKey:
      {
        // Cast to the derived message type
        MessageCharacterKey * key = (MessageCharacterKey*)m;

        //When different keys are pressed create different objects
        std::string ObjectToCreate;

        if( key->character == '1' )					
          ObjectToCreate = "Objects\\Ball.txt";
        else if( key->character == '2' )
          ObjectToCreate = "Objects\\Box.txt";
        else if( key->character == '3' )
          ObjectToCreate = "Objects\\Asteroid.txt";

        //if( !ObjectToCreate.empty() )
        //	CreateObjectAt(WorldMousePosition,0,ObjectToCreate);

        //Enable Physics Debugging
        if( key->character == ' ' )
          PHYSICS->AdvanceStep = true;
        if( key->character == ' ' && IsShiftHeld() )
          PHYSICS->StepModeActive = !PHYSICS->StepModeActive;

        if( key->character == 'g' )
        {
          ToggleDebugDisplay debugMessage( true );
          CORE->BroadcastMessage( &debugMessage );
        }

        if( key->character == 'h' )
        {
          ToggleDebugDisplay debugMessage( false );
          CORE->BroadcastMessage( &debugMessage );
        }
        break;
      }
    case Mid::FileDrop:
      {
        //FileDrop * drop = (FileDrop*)m;
        //LoadLevelFile(drop->FileName);
        break;
      }
    case Mid::MouseMove:
      {
        //The mouse has moved, update the mouse's world position
        MouseMove * mouseMove = (MouseMove*)m;
        GSM->WorldMousePosition = GRAPHICS->ScreenToWorldSpace(mouseMove->MousePosition);
        break;
      }
    }
  }

  ///Make asteroids, player, and a score object.
  void SinglePlayer::Initialize( void )
  {
    ///Load the assets of the level.
	  LoadLevelFile("Objects\\TestLevel.txt");
	  SpawnRandomAsteroids();
  } //Initialize

  ///Update the game, updates all systems.
  void SinglePlayer::Update( float dt )
  {
    ObjectLinkList<Controller>::iterator b = Controllers.begin(), e = Controllers.end();
    while (b != e)
    {
      b->Update(dt);
      ++b;
    }
  } //Update

  ///Cleanup assets and cleanup and ties to other states.
  void SinglePlayer::Cleanup( void )
  {
  } //Cleanup

  ///Reset all the positions, objects, scores, ect...
  void SinglePlayer::Restart( void )
  {
  } //Restart

  ///Creates an Object at the specified location. If a player, it'll save the ID. Controllers too.
  GOC * SinglePlayer::CreateObjectAt(Vec2& position,float rotation,const std::string& file)
	{
		//This is an example of using some data out of data file using
		//serialization and then override fields before the object
		//is initialized.

		//Build and serialize the object with data from the file
		GOC * newObject = FACTORY->BuildAndSerialize(file);
		//Get the transform and adjust the position
		Transform * transform = newObject->has(Transform);
		transform->Position = position;
		transform->Rotation = rotation;

    PlayerController * controller = newObject->has(PlayerController);
    if (controller)
      Controllers.push_back(controller);

		//Initialize the composition
		newObject->Initialize();

		return newObject;
	}
  
  ///Loads a level from a file. Doesn't unload current level.
	void SinglePlayer::LoadLevelFile(const std::string &filename)
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
			CreateObjectAt(objectPosition,objectRotation,"Objects\\" + objectArchetype);
		}
  } //LoadLevelFile

  ///Self explanatory... add a specific seed later...
	void SinglePlayer::SpawnRandomAsteroids()
  {
	  //^! if we end up with a level object then it should contain into like a number of random asteroids to spawn and the range
	  for (unsigned i = 0; i < 60; ++i)
	  {
		  float x_pos = 0;
		  float y_pos = 0;

		  // loop grabbing a random position not on top of the character
		  int safe_zone = 80;
		  int range = 700;
		  while (-safe_zone < x_pos && x_pos < safe_zone && -safe_zone < y_pos && y_pos < safe_zone)
		  {
			  x_pos = static_cast<float>(Utils::Random(-range, range));
			  y_pos = static_cast<float>(Utils::Random(-range, range));
		  }

		  CreateObjectAt(Vec2(x_pos, y_pos), static_cast<float>(Utils::Random(0, 360)), "Objects\\Asteroid.txt");
	  }
  } //SpawnRandomAsteroids

}
