#pragma once
#include "spritefactory.h"
#include "spaceship.h"
#include "SpriteRender.h"
class SpaceShipFactory :
	public SpriteFactory
{
public:
	SpaceShipFactory(LPDIRECT3DTEXTURE9 textureGroup, RECT rect);
	~SpaceShipFactory(void);
	GameObject* GetNewObject();
};
