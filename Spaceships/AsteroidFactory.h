#pragma once
#include "spritefactory.h"
#include "Asteroid.h"
#include "Collider.h"
#include "SpriteRender.h"

class AsteroidFactory :
	public SpriteFactory
{
public:
	AsteroidFactory(LPDIRECT3DTEXTURE9 textureGroup, RECT rect);
	~AsteroidFactory(void);
	virtual GameObject* GetNewObject();
	Collider* GetCollider();
	void SetFont(LPD3DXFONT font);
	
private:
	LPDIRECT3DTEXTURE9 _textures;
	RECT _rect;
	LPD3DXFONT font;

	
};
