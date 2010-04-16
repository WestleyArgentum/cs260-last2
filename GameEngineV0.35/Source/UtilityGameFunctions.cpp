
#include "Precompiled.h"
#include "UtilityGameFunctions.h"
#include "SinglePlayer.h"
#include "GameStateManager.h"

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

}