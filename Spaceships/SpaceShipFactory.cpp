#include "SpaceShipFactory.h"

SpaceShipFactory::SpaceShipFactory(LPDIRECT3DTEXTURE9 textureGroup, RECT rect) : SpriteFactory(textureGroup, rect)
{
	this->_rect = rect;
	this->_textures = textureGroup;
	this->initialID = 1;
	curID = initialID;
}

SpaceShipFactory::~SpaceShipFactory(void)
{	

}

GameObject* SpaceShipFactory::GetNewObject(){

	int nextID = IDFactory::GetNextID();
	Collider* c = new Collider();
	Mobile* m = new Mobile();
	m->SetValue(PhysicsComponent::PositionX, 200);
	m->SetValue(PhysicsComponent::PositionY, 200);
	
	c->SetRadius(10);
	SpaceShip* spaceship = new SpaceShip();
	spaceship->SetID(nextID);
	spaceship->SetMobile(m);
	m->SetOwner(spaceship);
	m->SetCollider(c);
	//TODO:  animation frames (someday)
	
	SpriteRender* sprite = new SpriteRender();

	sprite->SetTexture(_textures);
	sprite->SetDrawingRect(&_rect, 1);
	spaceship->SetRenderable((IRenderable*)sprite);
	spaceship->SetDamage(100);
	return spaceship;
}