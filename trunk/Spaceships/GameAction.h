/*
actions that the user is able to perform.  The game thinks of things in terms of game actions, so it doesn't care
what key the user pressed or whatever else happened, it just cares about if a command was issued to perform
one of these well-known actions.  Currently, not all of them are used necessarily.  If you need additional actions,
create them here
*/

#pragma once


enum GameAction{
	DO_NOTHING = 0, //do NOT change this.  the default value for DirectInput is 0 so mapping to 0
					//will cause whatever message is generated from it to be generated for every key that is
					//not pressed when the keyboard is triggered.  you will NEVER have an input event that
					//actually has a key ID of 0 that you need to map this against.
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	FIRE_PRIMARY,
	FIRE_SECONDARY,
	SELECT,
	STOP,
	QUIT,
	GameActionSIZE
};

