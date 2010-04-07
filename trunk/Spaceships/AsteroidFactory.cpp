#include "AsteroidFactory.h"


AsteroidFactory::AsteroidFactory(LPDIRECT3DTEXTURE9 textureGroup, RECT rect) : SpriteFactory(textureGroup, rect)
{
	this->_textures = textureGroup;
	this->_rect = rect;
	initialID = 1000;
	curID = initialID;
}

AsteroidFactory::~AsteroidFactory(void)
{
	
}

GameObject* AsteroidFactory::GetNewObject(void)
{

	//doesn't take much to destroy an asteroid but it sure does a lot of damage when it hits something
	int nextID = IDFactory::GetNextID();


	GameObject* asteroid = new Asteroid();
	SpriteRender* sprite = new SpriteRender();

	Collider* c = new Collider();
	Mobile* m = new Mobile();
	asteroid->SetMobile(m);

	m->ShouldBindToWorld(false);
	m->SetCollider(c);
	c->SetRadius(20);
	
	
	sprite->SetTexture(_textures);


	sprite->SetDrawingRect(&_rect, 1);
	asteroid->SetID(nextID);
	asteroid->SetRenderable((IRenderable*)sprite);	

	asteroid->GetMobile()->SetValue(PhysicsComponent::PositionX, 100);
	asteroid->GetMobile()->SetValue(PhysicsComponent::PositionY, 100);

	//these things live forever for now
	asteroid->SetLifetime(-1);	
	asteroid->SetDamage(300);
	asteroid->SetFont(font);

	
	return asteroid;
}

// TODO:  better design for this, feels like a hack
void AsteroidFactory::SetFont(LPD3DXFONT font)
{
	this->font = font;
}
