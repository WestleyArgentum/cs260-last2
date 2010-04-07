#include "precompiled.h"
#include "TextRenderer.h"

TextRenderer::TextRenderer(LPD3DXFONT font)
{
	this->font = font;
	this->color = D3DCOLOR_ARGB(255,255,255,255);
	width = 0;
	height = 0;
	text = NULL;
}

TextRenderer::~TextRenderer(void)
{

}

void TextRenderer::PreRender(View* view)
{
	
}

void TextRenderer::Render(View* view)
{
	if(font != NULL && text != NULL)
	{
		RECT rect;
		
		// don't forget to correct for current screen view
		// wouldn't it be nice if I could just set a flag to
		// do that and have the graphics engine take care of it?
		// maybe someday. . .
		rect.top =  PositionY() - view->GetViewRect().top;
		rect.left = PositionX() - view->GetViewRect().left ;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;
		int res = font->DrawText(view->GetSprite(), text, -1, &rect, DT_NOCLIP | DT_LEFT | DT_VCENTER, color);
		
		// 0 means failure
		if(res == 0)
			assert(false);
	}
}

void TextRenderer::PostRender(View* view)
{
	
}

void TextRenderer::SetText(LPCTSTR text)
{
	if(this->text != NULL)
	{
		delete this->text;
	}
	this->text = text;
}

void TextRenderer::SetColor(D3DCOLOR color)
{
	this->color = color;
}

void TextRenderer::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}