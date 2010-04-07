#include "GameController.h"
#include "bullet.h"
#include "spaceship.h"

#ifdef ONE_ASTEROID
bool oneAsteroid = true;
#endif

GameController::GameController(Model* m, View* v, AsteroidFactory* factory, SpaceShipFactory* sFactory) : Controller(m, v)
{
	this->_model = (GameModel*)m;
	this->_view = (GameView*)v;
	_model->SetCurrentController(this);
	this->newGameState = GameState::Playing;
	bulletLimiter = 0;
	this->asteroidFactory = factory;
	this->ShouldReset = true;	//trigger a reset the first time we pass through this
	this->sFactory = sFactory;
	// TODO: this feels like a hack
	// TODO: undid this hack with a #define.  note to self: fix this too
	this->physicsWorld.SetWorldSize(worldSizeX, worldSizeY);
	firstpass = true;
}

GameController::~GameController(void)
{

}

bool ShouldCreateAsteroid(float* x, float* y, float* rotation, float* velocity){

	//random number, generate an asteroid every 50 frames or so

	//if we should, decide where.
	int probability = RandomNumberGenerator::GetRandomNumber(1, 30);
	if(probability < 25)
		return false;

	//now we know to generate an asteroid.  Where to generate it?
	float rot = 0.0;
	int quadrant = RandomNumberGenerator::GetRandomNumber(1, 20);
	quadrant = quadrant % 4;
	rot = (float)RandomNumberGenerator::GetRandomNumber(2, 4);
	*rotation = rot / 10;

	switch(quadrant){
		case 0:
			//left side
			*x = -50;
			*y = (float)RandomNumberGenerator::GetRandomNumber(1, WORLD_SIZE_Y);
			*rotation = 0;
			//rotation should be between 0 and .5
			break;

		case 1:
			//top
			*x = (float)RandomNumberGenerator::GetRandomNumber(1, WORLD_SIZE_X);
			*y = -50;
			*rotation += .25;			
			break;

		case 2:
			//right
			*x = WORLD_SIZE_X + 100;
			*y = (float)RandomNumberGenerator::GetRandomNumber(1, WORLD_SIZE_Y);
			*rotation += .5;
			break;

		case 3:
			//bottom
			*x = (float)RandomNumberGenerator::GetRandomNumber(1, WORLD_SIZE_X);
			*y = WORLD_SIZE_Y + 100;
			*rotation -= .5;
			break;
	}

	*velocity = 3.0;//(float)RandomNumberGenerator::GetRandomNumber(1, 6);
	return true;
}

GameState GameController::Update()
{
	// detect first pass through rendering
	if(firstpass)
	{
		GlobalDebug::GetInstance()->DbgOut("first pass");
		firstpass = false;
	}

	//do we need to initialize?
	if(this->ShouldReset)
	{
		physicsWorld.Init();
		GameObject* s = this->sFactory->GetNewObject();
		s->SetController(this);
		// player always has ID 1
		s->SetID(1);
		s->GetMobile()->SetValue(PhysicsComponent::PositionX, 200);
		s->GetMobile()->SetValue(PhysicsComponent::PositionY, 200);
		this->_model->AddNewObject(s);
		/*physicsWorld.AddMobile(s->GetMobile());*/
		ShouldReset = false;
	}

	//game over?
	GameObject* player = _model->GetPlayerObject();
	if(player->IsDead()){
		//delete everything in the game state
		this->_model->Eradicate();
		this->ShouldReset = true;
		return GameState::MainMenu;
	}	

#ifndef NO_ASTEROIDS
	float xa, ya, ra, va;
	if(ShouldCreateAsteroid(&xa, &ya, &ra, &va)){
		GameObject* a = this->asteroidFactory->GetNewObject();
		a->GetMobile()->SetValue(PhysicsComponent::PositionX, xa);
		a->GetMobile()->SetValue(PhysicsComponent::PositionY, ya);
		a->GetMobile()->SetValue(PhysicsComponent::Rotation, ra);
		a->GetMobile()->SetValue(PhysicsComponent::GeneralVelocity, va);
		_model->AddNewObject(a);
	}
#endif

#ifdef ONE_ASTEROID
	if(oneAsteroid){
		GameObject* a = this->asteroidFactory->GetNewObject();
		a->GetMobile()->SetValue(PhysicsComponent::PositionX, 400);
		a->GetMobile()->SetValue(PhysicsComponent::PositionY, 400);
		a->GetMobile()->SetValue(PhysicsComponent::Rotation, .25);
		a->GetMobile()->SetValue(PhysicsComponent::GeneralVelocity, 2);
		a->SetController(this);
		_model->AddNewObject(a);
		oneAsteroid = false;
	}
#endif

	ProcessMessages();
	
	// update object state
	_model->UpdateObjects();
	this->ProcessMessages();


	// update physics
	physicsWorld.Update();
	

	// process my own messages in case I need to react to something that happened in 
	// physics or something
	this->ProcessMessages();

	// tell the view where the player is so that it can draw correctly
	_view->SetPlayerPosition(
		player->GetMobile()->GetValue(PhysicsComponent::PositionX),
		player->GetMobile()->GetValue(PhysicsComponent::PositionY),
		worldSizeX, worldSizeY);

	// handle collisions:
	// now the state of everything is updated, let's draw the current scene
	_view->DrawScreen();
	
	theReaper.DoWhatYouDo();
	
	return this->newGameState;
}

void GameController::PassMessage(Message* msg)
{
	this->messages.push_back(msg);
}

void GameController::ProcessMessages()
{
	while(messages.size() > 0)
	{
		Message* msg = messages.front();
		messages.pop_front();

		GameObject* obj = _model->GetPlayerObject();

		if(msg->GetMessageType() == MessageType::USER_INPUT)
		{
			assert(obj != NULL);
			obj->SendMessage(msg);
		}
		else if(msg->GetMessageType() == MessageType::GAME_OBJECT_ACTION)
		{
			if(msg->GetMessageContent() == MessageContent::CREATE_OBJECT)
			{
				GameObject* obj = (GameObject*)msg->GetExtended();
				assert(obj != NULL);
				this->_model->AddNewObject(obj);
				delete msg;
			}
			else if(msg->GetMessageContent() == MessageContent::DESTROY_OBJECT)
			{
				GameObject* obj = (GameObject*)msg->GetExtended();
				_model->DestroyObject( obj->GetID());
				theReaper.DoomObject((GameObject*)msg->GetExtended());
				delete msg;
			}
			else if(msg->GetMessageContent() == MessageContent::SHOULD_RENDER)
			{				 
				_view->PassMessage(msg);				 
			}
			else if(msg->GetMessageContent() == MessageContent::DO_PHYSICS)
			{
				physicsWorld.PassMessage(msg);
			}
		}
	}
	assert(messages.size() == 0);
}
