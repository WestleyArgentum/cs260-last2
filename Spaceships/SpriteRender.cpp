#include "precompiled.h"
#include "SpriteRender.h"

SpriteRender::SpriteRender(void)
{
	currentRect = 0;
	rects = NULL;
	texture = NULL;
	D3DXMatrixIdentity(&identity);
}

SpriteRender::SpriteRender(const SpriteRender& renderToCopy)
{
	currentRect = renderToCopy.currentRect;
	texture = renderToCopy.texture;
	rectCount = renderToCopy.rectCount;
	rects = new RECT[rectCount];
	memcpy(rects, renderToCopy.rects, sizeof(RECT) * rectCount);
}

SpriteRender::~SpriteRender(void)
{
	delete rects;
}

void SpriteRender::PreRender(View* view)
{
	// calculate current sprite center
	spriteCenter.x = abs(( rects[currentRect].left -  rects[currentRect].right)) / 2;
	spriteCenter.y = abs(( rects[currentRect].top -  rects[currentRect].bottom)) / 2;

	

	// set up rotation and transform (transform is actually into screen coordinates right now
	// but I may fix this eventually
	
	D3DXVECTOR2 pos(0,0);
	RECT rect = view->GetViewRect();

	if(this->positionX > rect.left)
	{
		pos.x = positionX - rect.left;
	}
	else
	{
		pos.x = positionX;
	}

	if(positionY > rect.top)
	{
		pos.y = positionY - rect.top;
	}
	else
	{
		pos.y = positionY;
	}

	// sprite coords are the upper-left, so correct them 
	// so that the sprite is centered over the 
	// position in the mobile.
	pos.x -= spriteCenter.x;
	pos.y -= spriteCenter.y;

	D3DXMatrixTransformation2D(&worldTransform, NULL, NULL, NULL, &spriteCenter, rotation, &pos);
}

void SpriteRender::Render(View* view)
{
	// translate position into world space
	view->GetSprite()->SetTransform(&worldTransform);
	// do we need to provide a center here?
	view->GetSprite()->Draw(texture, &rects[currentRect], NULL, NULL, D3DCOLOR_XRGB(255,255,255));	
}

void SpriteRender::PostRender(View* view)
{
	view->GetSprite()->SetTransform(&identity);
}

void SpriteRender::SetTexture(LPDIRECT3DTEXTURE9 txt)
{
	texture = txt;
}

void SpriteRender::SetDrawingRect(RECT* r, int count)
{
	rects = new RECT[count];
	this->rectCount = count;
	for(int i = 0; i < count; i++)
		rects[i] = r[i];
}