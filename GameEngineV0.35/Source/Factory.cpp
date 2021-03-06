///////////////////////////////////////////////////////////////////////////////////////
//
//	Factory.cpp
//	
//	Authors: Chris Peters, Benjamin Ellinger 
//	Copyright 2009, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "Factory.h"
#include "Composition.h"
#include "ComponentCreator.h"
#include "TextSerialization.h"

namespace Framework
{
	GameObjectFactory * FACTORY = NULL;

	GameObjectFactory::GameObjectFactory()
	{		
		ErrorIf( FACTORY!=NULL , "Factory already created");
		FACTORY = this;
		LastGameObjectId = 0;
	}

	GameObjectFactory::~GameObjectFactory()
	{
		//Delete all component creators
		ComponentMapType::iterator it = ComponentMap.begin();
		for(;it!=ComponentMap.end();++it)
			delete it->second;
	}

	GOC * GameObjectFactory::CreateEmptyComposition()
	{
		//Create the composition and give it an id
		//this function is used to create compositions
		//programmatically
		GOC * gameObject = new GOC();
		IdGameObject(gameObject, ++LastGameObjectId);
		return gameObject;	
	}

  GOC * GameObjectFactory::BuildAndSerialize( const std::string& filename )
  {
		//Id and initialize the game object composition
    return BuildAndSerialize( filename, ++LastGameObjectId );
  }

	GOC * GameObjectFactory::BuildAndSerialize( const std::string& filename, GOCId id )
	{
		//Open the text file stream serializer
		TextSerializer stream;
		bool fileOpened = stream.Open(filename);
		ErrorIf( !fileOpened , "Could not open file %s. File does not exist or is protected." , filename.c_str() );
		std::string componentName;

		//Make sure the stream is valid
		if( stream.IsGood() )
		{
			//Create a new game object to hold the components
			GOC * gameObject = new GOC();

			while(stream.IsGood())
			{
				//Read the component's name
				StreamRead(stream,componentName);

        if ( componentName == "" )
          continue;

				//Find the component's creator
				ComponentMapType::iterator it =  ComponentMap.find( componentName );
				ErrorIf( it == ComponentMap.end() , "Could not find component creator with name %s" , componentName.c_str() );
				if( it != ComponentMap.end() )
				{
					//ComponentCreator is an object that creates the component
					IComponentCreator * creator = it->second;

					//Create the component by using the interface
					GameComponent * component = creator->Create();

					//Serialize the component with current stream (this will move
					//the stream to the next component

					//Note: this invokes the base case of serialization in which
					//the object serializes itself. component->Serialize( stream );
					StreamRead(stream,*component);

					//Add the new component to the composition
					gameObject->AddComponent( componentName , component );
				}

        componentName.clear();
			}

	    if ( !IdGameObject(gameObject, id) )
      {
        delete gameObject;
        return NULL;
      }

      // set up the GOCType
      unsigned begin = filename.find_last_of("/\\") + 1;
      unsigned end = filename.find_last_of('.');
      gameObject->MyType = filename.substr(begin, end - begin);
			return gameObject;
		}

		return NULL;
	}

	bool GameObjectFactory::IdGameObject(GOC* gameObject, GOCId id)
	{
    GameObjectIdMapType::iterator it = GameObjectIdMap.find( id );

      // If the object hasn't been created yet!
    if ( it == GameObjectIdMap.end() )
    {
		  // Store the game object in the global object id map
      GameObjectIdMap[id] = gameObject;

      // Id the game object and return successful identification.
      gameObject->ObjectId = id;
      return true;
    }
    else
    {
      ErrorIf( true, "Error: Object already exists with the id specified!" );
      return false;
    }
	}

	GOC * GameObjectFactory::GetObjectWithId(GOCId id)
	{
		GameObjectIdMapType::iterator it = GameObjectIdMap.find(id);
		if( it!=GameObjectIdMap.end())
			return it->second;
		else
			return NULL;
	}

	GOC * GameObjectFactory::Create(const std::string& filename)
	{
		GOC * goc = BuildAndSerialize(filename);
		if(goc)
      goc->Initialize();

    return goc;
	}

	GOC * GameObjectFactory::Create( const std::string& filename, GOCId id )
	{
		GOC * goc = BuildAndSerialize( filename, id );
		if(goc)
      goc->Initialize();

    return goc;
	}

	void GameObjectFactory::SendMessage(Message * message)
  {}

	void GameObjectFactory::Destroy(GOC * gameObject)
	{
		//Add the object to the to be deleted list they will be delete
		//when the factory is updated
		ObjectsToBeDeleted.insert(gameObject);
	}

	void GameObjectFactory::DestroyById( GOCId gameObject )
	{
		if (GameObjectComposition* obj = GetObjectWithId(gameObject))
			ObjectsToBeDeleted.insert(obj);
	}

	void GameObjectFactory::Update(float dt)
	{
		//Delete all objects in the ObjectsToBeDeleted list 

		std::set<GOC*>::iterator it = ObjectsToBeDeleted.begin();
		for(;it!=ObjectsToBeDeleted.end();++it)
		{
			GOC * gameObject = *it;
			GameObjectIdMapType::iterator it =  GameObjectIdMap.find( gameObject->ObjectId );
			ErrorIf( it == GameObjectIdMap.end() , "Object %d was double deleted or is bad memory." , gameObject->ObjectId  );
			if( it!= GameObjectIdMap.end() )
			{
				//Delete it and remove its entry in the Id map
				delete gameObject;
				GameObjectIdMap.erase( it );
			}
		}
		//All objects to be delete have been deleted
		ObjectsToBeDeleted.clear();
	}

	//Bind the creator for this component with the component name
	void GameObjectFactory::AddComponentCreator(const std::string& name,IComponentCreator* creator)
	{
		ComponentMap[name] = creator;
	}

	//Clean up the game world
	void GameObjectFactory::DestroyAllObjects()
	{
		GameObjectIdMapType::iterator it =  GameObjectIdMap.begin();
		for(;it!=GameObjectIdMap.end();++it)
			delete it->second;

		GameObjectIdMap.clear();
	}
}