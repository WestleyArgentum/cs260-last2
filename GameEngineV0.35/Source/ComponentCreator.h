///////////////////////////////////////////////////////////////////////////////////////
///
///	\file ComponentCreator.h Defines IComponentCreator interface.
///
///	Authors: Chris Peters, Benjamin Ellinger 
///	Copyright 2009, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace Framework
{
	//Forward declaration of component
	class GameComponent;

	///Game Component Creator Interface.
	///Used to create components in data driven method in the GameObjectFactory.
	class IComponentCreator
	{
	public:
		///Create the component
		virtual GameComponent * Create()=0;
	};

	///Templated helper class used to make concrete component creators.
	template<typename type>
	class ComponentCreatorType : public IComponentCreator
	{
		virtual GameComponent * Create()
		{
			return new type();
		}
	};

};

//Register component macro
#define RegisterComponent(type) FACTORY->AddComponentCreator( #type, new ComponentCreatorType<type>() );