///////////////////////////////////////////////////////////////////////////////////////
///
///	\file GameLogic.h
///	Sample game logic for demo.
///	
///	Authors: Benjamin Ellinger, Chris Peters
///	Copyright 2009, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////

#pragma once //Makes sure this header is only included once

#include "Graphics.h"
#include "Physics.h"
#include "Engine.h"
#include "IController.h"

namespace Framework
{
	///Sample Demo Game Logic
	class GameLogic : public ISystem
	{
	public:	
		GameLogic();
		~GameLogic();
		void Initialize();
		void Update(float timeslice);
		virtual std::string GetName(){return "GameLogic";}
		virtual void SendMessage(Message *);
		GOC * CreateObjectAt(Vec2& position,float rotation,const std::string& file);
		void LoadLevelFile(const std::string& file);

		Vec2 WorldMousePosition;

	public:
		ObjectLinkList<Controller> Controllers;

		void SpawnRandomAsteroids();
	};

	//A global pointer to our game so that it can be accessed from anywhere.
	extern GameLogic* LOGIC;
}


