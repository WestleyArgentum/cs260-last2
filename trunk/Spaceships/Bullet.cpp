#include "Bullet.h"

Bullet::Bullet()
{
	this->SetLifetime(bullet_lifetime);

	IPrimitiveRenderable* points = new IPrimitiveRenderable(4);
	
	D3DXVECTOR3 localoffset;
	localoffset.x = 5;
	localoffset.y = 5;
	
	Vertex v;
	v.color = D3DCOLOR_COLORVALUE( 0.5, 1.0, 0.0, 1.0 );
	v.x = 0;
	v.y = 0;
	v.z = 0;
	points->AddVertex(v);

	v.color = D3DCOLOR_COLORVALUE( 0.5, 1.0, 0.0, 1.0 );
	v.x = 1;
	v.y = 0;
	v.z = 0;
	points->AddVertex(v);

	v.color = D3DCOLOR_COLORVALUE( 0.5, 1.0, 0.0, 1.0 );
	v.x = 0;
	v.y = 1;
	v.z = 0;
	points->AddVertex(v);
	
	v.color = D3DCOLOR_COLORVALUE( 0.5, 1.0, 0.0, 1.0 );
	v.x = 1;
	v.y = 1;
	v.z = 0;
	points->AddVertex(v);
	renderable = (IRenderable*)points;
	hitPoints = 1;

}

Bullet::~Bullet(void)
{
	if(renderable != NULL)
	{
		renderable = NULL;
		delete renderable;
	}
}

bool Bullet::IsDead()
{

	if(hitPoints <= 0)
		return true;

	else if(_lifeTime == 0)
		return true;
	else
		return false;
}

// TODO:  why does this return a bool?
void Bullet::OnUpdate()
{
	//increment this thing's life
	this->KillSlowly(); 
}

void Bullet::PreProcessMessages()
{

}
void Bullet::PostProcessMessages()
{
	
}
void Bullet::ProcessMessage(Message* msg)
{
	if(msg->GetMessageType() == MessageType::GAME_OBJECT_ACTION)
	{
		// kill this if it hits anything no matter what it hits
		if(msg->GetMessageContent() == MessageContent::COLLISION)
		{
			this->hitPoints = 0;
		}
	}
	delete msg;
}

void Bullet::PreGetRenderable()
{
	renderable->SetPosition(
		mobile->GetValue(PhysicsComponent::PositionX),
		mobile->GetValue(PhysicsComponent::PositionY));
}

ObjectType Bullet::GetType()
{
	return ObjectType::BULLET;
}