///////////////////////////////////////////////////////////////////////////////
///
///	\file Factory.h
///	Defines the GameObjectFactory system.
///	
///	Authors: Chris Peters, Benjamin Ellinger 
///	Copyright 2009, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "System.h"
#include "Composition.h"

namespace Framework
{
	///Forward Declaration of Component Creator
	class IComponentCreator;

	///Game Object Factory The game object factory creates composition objects 
	///from data streams and manages their lifetimes. As part of controlling the 
	///lifetimes of the GOC it also provides an integer based Id system for safe 
	///referencing of game objects through integer Id Handles.
	class GameObjectFactory : public ISystem
	{
  public:
		typedef stdext::hash_map<unsigned,GOC*> GameObjectIdMapType;
	public:
		GameObjectFactory();
		~GameObjectFactory();

		///Create initialize and Id a GOC from the data file.
		GOC * Create(const std::string& filename);
	  GOC * Create( const std::string& filename, GOCId id );

		///Add a GOC to the destroy list for delayed destruction.
		void Destroy(GOC * gameObject);
		void DestroyById(GOCId gameObject);

		///Update the factory, destroying dead objects.
		virtual void Update(float dt);

		///Name of the system is factory.
		virtual std::string GetName(){return "Factory";}

		///Message Interface see Message.h
		virtual void SendMessage(Message * message);

		///Destroy all the GOCs in the world. Used for final shutdown.
		void DestroyAllObjects();

		///Create and Id a GOC at runtime. Used to dynamically build GOC.
		///After components have been added call GOC->Initialize().
		GOC * CreateEmptyComposition();
    GOC * CreateEmptyComposition( GOCId id );

		///Build a composition and serialize from the data file but do not initialize the GOC.
		///Used to create a composition and then adjust its data before initialization
		///see GameObjectComposition::Initialize for details.
		GOC * BuildAndSerialize(const std::string& filename);
    GOC * BuildAndSerialize(const std::string& filename, GOCId id );

		///Id object and store it in the object map.
    bool IdGameObject(GOC* gameObject, GOCId id);

		///Add a component creator enabling data driven composition
		void AddComponentCreator(const std::string& name,IComponentCreator* creator);

		///Get the game object with given id. This function will return NULL if
		///the object has been destroyed.
		GOC * GetObjectWithId(GOCId id);

    GameObjectIdMapType::iterator begin( void ) { return GameObjectIdMap.begin(); }
    GameObjectIdMapType::const_iterator end( void ) const { return GameObjectIdMap.end(); }

	private:
		///Used to incrementally generate unique id's.
		unsigned LastGameObjectId;

		///Map of component creator used for data driven composition
		typedef std::map<std::string,IComponentCreator*> ComponentMapType;
		ComponentMapType ComponentMap;

		///Map of GOC to their Ids used for safe referencing of game objects
		GameObjectIdMapType GameObjectIdMap;

		///Objects to be deleted
		///this is a set to prevent problems when a game object
		///is deleted in multiple locations
		std::set<GOC*> ObjectsToBeDeleted;

	};

	extern GameObjectFactory * FACTORY;
}
