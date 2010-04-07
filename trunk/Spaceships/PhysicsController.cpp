#include "PhysicsController.h"
#include "gamestate.h"

PhysicsController::PhysicsController(Model* m, View* v):Controller(m, v)
{
}

PhysicsController::~PhysicsController(void)
{
}

void PhysicsController::HandleMessages(list<Message*>* l)
{

}
GameState PhysicsController::Update()
{
	return GameState::Playing;

}
void PhysicsController::PassMessage(Message* msg)
{

}
