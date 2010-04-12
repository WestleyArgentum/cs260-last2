///////////////////////////////////////////////////////////////////////////////////////
//
//	Game Logic
//	Simple game logic for demo. A good way to approach understand the game sample
//	is to walk through all the code in game logic and follow the execution through
//	the rest of the engine.
//	
//	Authors: Chris Peters, Benjamin Ellinger
//	Copyright 2009, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "GameLogic.h"
#include "WindowsSystem.h"
#include "Core.h"
#include "ComponentCreator.h"
#include "Camera.h"
#include "TextSerialization.h"
#include "DebugDraw.h"
#include "PlayerController.h"
#include "PRNG.h"
#include "Asteroid.h"

#include <ctime>


namespace Framework
{
	//Our global pointer to the game.
	GameLogic* LOGIC = NULL;

	void GameLogic::Initialize()
	{
		//Components are explicitly registered in initialize
		RegisterComponent(Transform);
		//This macro expands into FACTORY->AddComponentCreator( "Transform" , new ComponentCreatorType<Transform>()  );

		RegisterComponent(PlayerController);
		RegisterComponent(Asteroid);

		const bool UseLevelFile = true;

		if( UseLevelFile )
		{
			LoadLevelFile("Objects\\TestLevel.txt");

			// for now we have no state manager so I'll do this here
			SpawnRandomAsteroids();
		}
		else
		{
			//The following sections of code show what is looks like to compose the game object
			//in code vs using data driven composition. Normally you will use data driven
			//composition, but there are cases where you might want to do it in code.
			const bool DataDrivenObjects = true;

			//Create the camera
			if(DataDrivenObjects)
			{
				FACTORY->Create( "Objects\\Camera.txt" );
			}
			else
			{
				/*GOC * camera = FACTORY->CreateEmptyComposition();
				camera->AddComponent( "Transform" , new Transform() );
				Controller * controller = new Controller();
				controller->Speed = 20.0f;
				camera->AddComponent( "Controller" , controller );
				camera->AddComponent( "CameraView" , new Camera() );
				camera->Initialize();*/
			}

			//Create the ground
			if(DataDrivenObjects)
			{
				FACTORY->Create( "Objects\\Ground.txt" );
			}
			else
			{
				GOC * gameObject = FACTORY->CreateEmptyComposition();
				Transform * transform = new Transform();
				transform->Position = Vec2(0,-300);;
				gameObject->AddComponent( "Transform" , transform );
				Body * body = new Body();
				body->Mass = 0.0f;
				body->Restitution = 0.3f;
				body->Friction = 0.3f;
				ShapeAAB * box = new ShapeAAB();
				box->Extents = Vec2(300,10);
				body->BodyShape = box;		
				gameObject->AddComponent( "Body" , body );

				Sprite * sprite = new Sprite();
				sprite->SpriteName = "square";
				sprite->Size = Vec2(600,20);
				sprite->Color = Vec4(1,0,0,1);
				gameObject->AddComponent( "Sprite" , sprite );

				gameObject->Initialize();
			}

			//Create the two bumper walls
			FACTORY->Create( "Objects\\WallLeft.txt" );
			FACTORY->Create( "Objects\\WallRight.txt" );
			
		}

	}

	GameLogic::GameLogic()
	{	
		//Set up the global pointer
		ErrorIf(LOGIC!=NULL,"Logic already initialized");
		LOGIC = this;

	}

	GameLogic::~GameLogic()
	{
	}

	void GameLogic::SendMessage(Message * m )
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

					if( !ObjectToCreate.empty() )
						CreateObjectAt(WorldMousePosition,0,ObjectToCreate);

					//Enable Physics Debugging
					if( key->character == ' ' )
						PHYSICS->AdvanceStep = true;
					if( key->character == ' ' && IsShiftHeld() )
						PHYSICS->StepModeActive = !PHYSICS->StepModeActive;

					if( key->character == 'd' )
					{
						ToggleDebugDisplay debugMessage( true );
						CORE->BroadcastMessage( &debugMessage );
					}

					if( key->character == 'f' )
					{
						ToggleDebugDisplay debugMessage( false );
						CORE->BroadcastMessage( &debugMessage );
					}
					break;
				}
			case Mid::FileDrop:
				{
					FileDrop * drop = (FileDrop*)m;
					LoadLevelFile(drop->FileName);
					break;
				}
			case Mid::MouseMove:
				{
					//The mouse has moved, update the mouse's world position
					MouseMove * mouseMove = (MouseMove*)m;
					WorldMousePosition = GRAPHICS->ScreenToWorldSpace(mouseMove->MousePosition);
					break;
				}
		}

	};

	void GameLogic::Update(float dt)
	{
		ObjectLinkList<Controller>::iterator it = Controllers.begin();
		for(;it!=Controllers.end();++it)
			it->Update(dt);

    //Drawer::Instance.DrawCircle( Vec2(0, 0), 3 );
	}
	GOC * GameLogic::CreateObjectAt(Vec2& position,float rotation,const std::string& file)
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

		//Initialize the composition
		newObject->Initialize();

		return newObject;
	}

	void GameLogic::LoadLevelFile(const std::string& filename)
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
	}

	void GameLogic::SpawnRandomAsteroids()
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
	}

}


