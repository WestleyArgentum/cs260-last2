#include "SpriteFactory.h"

SpriteFactory::SpriteFactory(LPDIRECT3DTEXTURE9 textureGroup, RECT rect)
{
	InitializeCriticalSection(&this->safeID);
	this->_textures = textureGroup;
	this->_rect = rect;
}

SpriteFactory::~SpriteFactory(void)
{
	DeleteCriticalSection(&this->safeID);
}

void SpriteFactory::ResetID(){

	EnterCriticalSection(&this->safeID);
	curID = initialID;
	LeaveCriticalSection(&this->safeID);

}
