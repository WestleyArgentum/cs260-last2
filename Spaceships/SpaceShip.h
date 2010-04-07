#pragma once
#include "Weapon.h"

#define SpaceShipBaseHitpoints  100;
const float spaceShipVelocity = 6;



class SpaceShip :
	public GameObject
{
public:
	SpaceShip();
	~SpaceShip(void);
	virtual bool IsDead();
	virtual float GetGunPositionX();
	virtual float GetGunPositionY();
	GameObject* FirePrimary();
	GameObject* FireSecondary();
	virtual ObjectType GetType();
	void SetPrimaryWeapon(IWeapon* weapon);
protected:
	void PreProcessMessages();
	void ProcessMessage(Message* msg);
	void PostProcessMessages();
	void PreGetRenderable();
	void OnUpdate();

private:
	int hitpoints;
	int rateOfFire;
	int bulletCounter;
	IWeapon* primaryWeapon;
	IWeapon* secondaryWeapon;
};
