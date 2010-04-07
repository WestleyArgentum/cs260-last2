#include "GameModel.h"

GameModel::GameModel(void)
{	
}

void GameModel::Eradicate(){
	//destroy everything
	while(_objects.size() > 0)
	{
		GameObject* obj = _objects.front();
		_objects.pop_front();
		delete obj;
	}	
}

GameModel::~GameModel(void)
{
		Eradicate();
}


//ID of the main player for this game.  
GameObject* GameModel::GetPlayerObject() const
{
	return this->FindObjectById(1);
}

// destroyobject removes an object's reference from the model
// but does NOT destroy the object itself so the object may potentially
// be re-used.
void GameModel::DestroyObject(const int objectId){
	// go-go gadget STL!
	list<GameObject*>::iterator iter;
	iter = remove_if(
			_objects.begin(),
			_objects.end(),
			bind2nd(GameObjectFinder(), objectId)),
		_objects.end();

	_objects.erase(iter);
}


GameObject* GameModel::FindObjectById(const int objectId) const
{
	return (GameObject*)*find_if(
		_objects.begin(), 
		_objects.end(), 
		bind2nd(GameObjectFinder(), objectId));
}

//add a new object of some type
void GameModel::AddNewObject(GameObject* obj)
{	
	this->_objects.push_back(obj);
}

//update non-player objects
void GameModel::UpdateObjects()
{
	//check all object state
	list<GameObject*>::iterator objIter;
	
	for(objIter = _objects.begin(); objIter != _objects.end(); ++objIter)
	{
		GameObject* cur = (GameObject*)*objIter;
		assert(cur != NULL);
		cur->Update();

		// delete anything that is dead.  It may have been killed on the previous pass through the game loop by the 
		// collider
		// TODO:  no deaths actually should happen here
		if(cur->IsDead())
		{
			// tell the controller to remove me from the list of objects that exist
			Message* msg = new Message(MessageType::GAME_OBJECT_ACTION, MessageContent::DESTROY_OBJECT, (void*)cur);
			controller->PassMessage(msg);
		}
		else
		{
			// not dead, add it to the list of things that need to be drawn
			IRenderable* renderable = cur->GetRenderable();
			if(renderable != NULL)
			{
				Message* msg = 
					new Message(
					MessageType::GAME_OBJECT_ACTION,
					MessageContent::SHOULD_RENDER, 
					(void*)renderable);

				// tell my controller that I should be rendered if I'm on the screen
				this->controller->PassMessage(msg);			
			}				

			Mobile* mobile = cur->GetMobile();
			// can this thing move?  remember, just because a thing
			// doesn't move (i.e. velocity = 0) doesn't mean that it 
			// can't move
			if(mobile != NULL)
			{
				Message* msg = 
					new Message(
					MessageType::GAME_OBJECT_ACTION,
					MessageContent::DO_PHYSICS, 
					(void*)mobile);

				// tell my controller that I should be sent to the physics controller
				// to have it do physics on me.
				this->controller->PassMessage(msg);
			}
		}
	}
}