#pragma once

class SpriteFactory
{
public:
	SpriteFactory(LPDIRECT3DTEXTURE9 textureGroup, RECT rect);
	~SpriteFactory(void);
	virtual GameObject* GetNewObject() = 0;
	void ResetID();
protected:
	CRITICAL_SECTION safeID;
	int curID;
	int initialID;
	LPDIRECT3DTEXTURE9 _textures;
	RECT _rect;
};
