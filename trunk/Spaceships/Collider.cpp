#include "Collider.h"

Collider::Collider()
{

	owner = NULL;

}
//
//void Collider::SetParentID(int id)
//{
//	this->parentID = id;
//}

int Collider::GetParentID()
{
	return 0;
}

int Collider::GetOwnerID()
{
	return this->owner->GetID();
}

Collider::~Collider(void)
{
}

bool Collider::CollidedWith(Collider* obj){
	//did I hit the thing?

	//calculate the x distance

	float x1 = abs(obj->x - this->x);

	//calculate the y distance
	float y1 = abs(obj->y - this->y);

	x1 = x1*x1;
	y1 = y1*y1;
	
	//calculate the radius and see if it's less than the distance
	float rad = this->radius > obj->radius ? this->radius : obj->radius;

	rad = rad*rad;

	if((x1 + y1) < rad)
	{

#ifdef COLLISION_DEBUG
	GlobalDebug::GetInstance()->DbgOutFloat("\r\n\r\nCollider X:", this->x);
	GlobalDebug::GetInstance()->DbgOutFloat("\r\nCollider Y:", this->y);
	GlobalDebug::GetInstance()->DbgOutFloat("\r\nCollider radius:", this->radius);
	GlobalDebug::GetInstance()->DbgOutFloat("\r\nCollideswith X:", obj->x);
	GlobalDebug::GetInstance()->DbgOutFloat("\r\nCollideswith Y:", obj->y);
	GlobalDebug::GetInstance()->DbgOutFloat("\r\nCollideswith radius:", obj->radius);
#endif

		return true;
	}else
		return false;
}

int Collider::DoDamage(Collider* obj){
	obj->SetDamage(this->damage);
	return this->damage;
}

int Collider::SetDamage(int d){
	this->hitPoints -= d;
	return this->hitPoints;
}

void Collider::SetPosition(float x, float y){
	this->x = x;
	this->y = y;
}

int Collider::GetDamage()
{
	return this->damage;
}

int Collider::GetHitpoints()
{
	return this->hitPoints;
}

void Collider::SetRadius(int r)
{
	this->radius = r;
}

void Collider::SetOwner(GameObject* object)
{
	owner = object;	
}

GameObject* Collider::GetOwner()
{
	assert(owner != NULL);
	return owner;
}