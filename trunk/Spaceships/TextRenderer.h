#pragma once
#include "RenderableBase.h"
class TextRenderer :
	public RenderableBase
{
public:
	TextRenderer(LPD3DXFONT font);
	~TextRenderer(void);
	
	virtual void PreRender(View* view);

	virtual void Render(View* view);

	virtual void PostRender(View* view);

	virtual void SetText(LPCTSTR text);
	virtual void SetColor(D3DCOLOR color);

	virtual void SetSize(int width, int height);
private:
	LPCSTR textToRender;
	LPD3DXFONT font;
	LPCTSTR text;
	D3DCOLOR color;
	int width;
	int height;
};

