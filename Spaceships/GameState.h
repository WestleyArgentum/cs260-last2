#pragma once

//the state of the game at any given moment.

const int GameStateCount = 3;

//TODO:  add a state for when the user is on the networking menu.
enum GameState{
	MainMenu,
	Playing,
	GameOver,
	Quit
};