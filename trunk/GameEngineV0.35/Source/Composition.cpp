///////////////////////////////////////////////////////////////////////////////////////
//
//	Composition.cpp
//	Authors: Chris Peters, Benjamin Ellinger 
//	Copyright 2009, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
#include "Precompiled.h"
#include "Composition.h"
#include "Factory.h"

namespace Framework
{
	void GameObjectComposition::Initialize()
	{
		//Initialize every component on this composition
		//and set their composition owner. This allows each
		//component to initialize itself separate from its constructor.
		//Which is need for serialization and to allow components to get
		//pointers to each other during initialization.
		for( MapIt it = Map.begin();it!=Map.end();++it)
		{	
			it->second->Base = this;
			it->second->Initialize();
		}
	}

	GameObjectComposition::GameObjectComposition()
	{
		ObjectId = 0;
	}

	GameObjectComposition::~GameObjectComposition()
	{
		//Delete each component using the component's virtual destructor
		//takes care of all resources and memory.
		for( MapIt it = Map.begin();it!=Map.end();++it)
			delete it->second;
	}

	void GameObjectComposition::AddComponent(const std::string& componentName,GameComponent * component)
	{
		//Store the component on the component map using it name as a key
		Map[componentName] = component;
	}

	void GameObjectComposition::SendMessage(Message * message)
	{
		//When a message is sent to a composition it sends to
		//all of its components
		for( MapIt it = Map.begin();it!=Map.end();++it)
			it->second->SendMessage(message);
	}

	GameComponent * GameObjectComposition::GetComponent(const std::string& name)
	{
		//Attempt to find the component on the component map if
		//the component is not found return NULL
		MapIt it = Map.find(name);
		if( it != Map.end() )
			return it->second;
		else
			return NULL;
	}

	void GameObjectComposition::Destroy()
	{ 
		//Signal the factory that is object needs to be destroyed
		//this will happen at the end of the frame.
		FACTORY->Destroy(this); 
	}
}
