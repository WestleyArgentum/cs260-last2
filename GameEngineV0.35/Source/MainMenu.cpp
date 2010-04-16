#include "Precompiled.h"
#include "MainMenu.h"
#include "MessageHub.h"
#include "WindowsSystem.h"
#include "GameStateManager.h"

namespace Framework
{
  MainMenu::MainMenu( GameStateManager *gsm ) : IGameState(gsm)
  {
  }

  MainMenu::~MainMenu( void )
  {
  }

  // Adds a controller to the internal list of the GameState.
  void MainMenu::AddController( Controller *controller )
  {
    Controllers.push_back(controller);
  }

  // Removes a controller from the internal list of the GameState.
  void MainMenu::RemoveController( Controller *controller )
  {
    Controllers.erase(controller);
  }

  // Standard send message function.
  void MainMenu::SendMessage( Message *message )
  {
    switch ( message->MessageId )
    {
      case Mid::CharacterKey:
        {
          break;
        }

      case Mid::MouseMove:
        {
          MouseMove *mouseMove = static_cast<MouseMove*>(message);
          gsm_->WorldMousePosition = GRAPHICS->ScreenToWorldSpace(mouseMove->MousePosition);
          break;
        }
    }
  }

  // Make asteroids, player, and a score object.
  void MainMenu::Initialize( void )
  {
	  LoadFromFile("Levels\\MainMenu.txt");
  }

  // Update the game, updates all systems.
  void MainMenu::Update( float dt )
  {
    for ( ControllerList::iterator it = Controllers.begin(); it != Controllers.end(); ++it )
    {
      it->Update(dt);
    }
  }

  // Cleanup assets and cleanup and ties to other states.
  void MainMenu::OnCleanup( void )
  {
  }

  // Reset all the positions, objects, scores, ect...
  void MainMenu::Restart( void )
  {
  }

}   // namespace Framework

