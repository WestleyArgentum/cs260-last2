
#include "Precompiled.h"
#include "UtilityGameFunctions.h"
#include "SinglePlayer.h"
#include "GameStateManager.h"
#include "PRNG.h"

namespace Framework
{

	///Creates an Object at the specified location.
	GOC * CreateObjectAt(Vec2& position,float rotation,const std::string& file)
	{
		//This is an example of using some data out of data file using
		//serialization and then override fields before the object
		//is initialized.

		//Build and serialize the object with data from the file
		GOC * newObject = FACTORY->BuildAndSerialize("Objects\\" + file + ".txt");
		//Get the transform and adjust the position
		Transform * transform = newObject->has(Transform);
		transform->Position = position;
		transform->Rotation = rotation;

		//Initialize the composition
		newObject->Initialize();

		GSM->RegisterComposition(newObject->GetId());

		return newObject;
	}

  GOC * CreateObjectAt(Vec2& position,float rotation,const std::string& file, GOCId id)
	{
		//This is an example of using some data out of data file using
		//serialization and then override fields before the object
		//is initialized.

		//Build and serialize the object with data from the file
		GOC * newObject = FACTORY->Create("Objects\\" + file + ".txt", id);
		//Get the transform and adjust the position
		Transform * transform = newObject->has(Transform);
		transform->Position = position;
		transform->Rotation = rotation;

		//Initialize the composition
		//newObject->Initialize();

		GSM->RegisterComposition(newObject->GetId());

		return newObject;
	}

	///Self explanatory... add a specific seed later...
	void SpawnRandomAsteroids()
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

			CreateObjectAt(Vec2(x_pos, y_pos), static_cast<float>(Utils::Random(0, 360)), "Asteroid");
		}
	} //SpawnRandomAsteroids

}