#include "precompiled.h"
#include "ObjectReaper.h"

ObjectReaper::ObjectReaper(void)
{
}

ObjectReaper::~ObjectReaper(void)
{
	// make sure that we clean up after ourselves if there
	// are objects queued up
	DoWhatYouDo();
}

void ObjectReaper::DoomObject(GameObject* object)
{
	objectsToDestroy.push_back(object);
}

// what it does is destroy objects
void ObjectReaper::DoWhatYouDo()
{
	while(objectsToDestroy.size() > 0)
	{
		GameObject* obj = objectsToDestroy.front();
		objectsToDestroy.pop_front();
		delete obj;		
	}
	assert(objectsToDestroy.size() == 0);
}