#pragma once
#include "RenderableBase.h"

class SpriteRender :
	public RenderableBase
{
public:
	SpriteRender(void);
	SpriteRender(const SpriteRender& renderToCopy);
	~SpriteRender(void);
	virtual void PreRender(View* view);
	virtual void Render(View* view);
	virtual void PostRender(View* view);
	void SetTexture(LPDIRECT3DTEXTURE9 txt);
	void SetDrawingRect(RECT* r, int count);
private:
	
	// number of sprites in this thing's animation
	int rectCount;	

	// positions of sprites in the supplied texture
	RECT* rects;	

	// rect we're on right now
	int currentRect;	

	// texture that holds the sprite and any animation frames it may have
	LPDIRECT3DTEXTURE9 texture;

	// size in pixels
	int sizex, sizey;

	
	
	D3DXMATRIX worldTransform;
	D3DXMATRIX identity;
	D3DXVECTOR2 spriteCenter;
};

