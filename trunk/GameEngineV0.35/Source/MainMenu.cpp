#include "Precompiled.h"
#include "MainMenu.h"
#include "MessageHub.h"
#include "WindowsSystem.h"
#include "GameStateManager.h"
#include "Core.h"

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
      case Mid::WindowKey:
        {
          MessageWindowKey *key = static_cast<MessageWindowKey*>( message );
          int index = static_cast<int>( index_ );

          switch ( key->key_ )
          {
            case VK_RETURN:
              switch ( index_ )
              {
                case LI_SinglePlayer:
                  gsm_->ChangeState( "SinglePlayer" );
                  break;
                case LI_HostGame:
                  // Host game state change here
                  gsm_->ChangeState( "ServerState" );
                  break;
                case LI_JoinGame:
                  // Join game state change here
                  break;
                case LI_Quit:
                  MessageQuit q;
                  CORE->BroadcastMessage( &q );
                  break;
              }
              break;

            case VK_DOWN:
              ++index;

              if ( index >= LI_NumberOfIndexes )
              {
                index = 0;
              }
              break;
            case VK_UP:
              --index;

              if ( index < 0 )
              {
                index = LI_NumberOfIndexes - 1;
              }
              break;
          }

          index_ = static_cast<LevelIndex>( index );
          break;
        }

      case Mid::CharacterKey:
        {
          MessageCharacterKey *key = static_cast<MessageCharacterKey*>( message );

          if( key->character == 'g' )
          {
            ToggleDebugDisplay debugMessage( true );
            CORE->BroadcastMessage( &debugMessage );
          }

				  else if( key->character == 'h' )
          {
            ToggleDebugDisplay debugMessage( false );
            CORE->BroadcastMessage( &debugMessage );
          }
          break;
        }

        // Need to add DisplayText component
        // Add body to text to test with collision with mouse!

      case Mid::MouseMove:
        {
          MouseMove *mouseMove = static_cast<MouseMove*>(message);
          gsm_->WorldMousePosition = GRAPHICS->ScreenToWorldSpace(mouseMove->MousePosition);
          break;
        }
    }
  }

  namespace
  {
      // Set the text of this particular object to the text string passed in! (Similar to printf)
    void SetText( GOC *textObject, const char *text )
    {
      if ( textObject )
      {
        Text *textComponent = textObject->has(Text);

        if ( textComponent )
        {
          textComponent->SetText( text );
        }
      }
    }

      // Set the color of the text object
    void SetTextColor( GOCId id, unsigned char red, unsigned char green, unsigned char blue,
      unsigned char alpha )
    {
      GOC *textObject = FACTORY->GetObjectWithId( id );

      if ( textObject )
      {
        Text *textComponent = textObject->has(Text);

        if ( textComponent )
        {
          textComponent->SetColor( red, green, blue, alpha );
        }
      }
    }
  }

  // Make asteroids, player, and a score object.
  void MainMenu::Initialize( void )
  {
	  LoadFromFile( "Levels\\MainMenu.txt" );

    // Load in all menu text so we can choose which level to load next.
    GOC *singlePlayer = CreateObjectAt( Vec2(20,  10), 0, "MenuText" );
    GOC *hostGame     = CreateObjectAt( Vec2(20, 120), 0, "MenuText" );
    GOC *joinGame     = CreateObjectAt( Vec2(20, 230), 0, "MenuText" );
    GOC *quit         = CreateObjectAt( Vec2(20, 340), 0, "MenuText" );

    SetText( singlePlayer, "Single Player" );
    SetText( hostGame    , "Host Game" );
    SetText( joinGame    , "Join Game" );
    SetText( quit        , "Quit" );

    index_ = LI_SinglePlayer;

    indexes_[LI_SinglePlayer] = singlePlayer->GetId();
    indexes_[LI_HostGame]     = hostGame->GetId();
    indexes_[LI_JoinGame]     = joinGame->GetId();
    indexes_[LI_Quit]         = quit->GetId();
  }

  // Update the game, updates all systems.
  void MainMenu::Update( float dt )
  {
    for ( unsigned i = 0; i < LI_NumberOfIndexes; ++i )
    {
      SetTextColor( indexes_[i], 100, 100, 100, 255 );
    }

      // Highlight the current text index the player is on.
    SetTextColor( indexes_[index_], 255, 255, 255, 255 );

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

