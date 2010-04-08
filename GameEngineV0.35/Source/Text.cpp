#include "Precompiled.h"
#include "Text.h"
#include "Transform.h"
#include "Graphics.h"
#include "Camera.h"
#include "DebugDraw.h"

struct Vec4int
{
  int x, y, z, w;
};

namespace Framework
{
	inline void StreamRead( ISerializer &stream, Vec4int &v )
	{
		StreamRead( stream, v.x );
		StreamRead( stream, v.y );
		StreamRead( stream, v.z );
		StreamRead( stream, v.w );
	}

  Text::Text( void )
  {
  }

  Text::~Text( void )
  {
    font_->Release();

    GRAPHICS->TextList.erase( this );
  }

  void Text::Initialize( void )
  {
    transform = GetOwner()->has( Transform );

    GRAPHICS->TextList.push_back( this );
  }

  void Text::Serialize( ISerializer &stream )
  {
    Vec4int tempcolor;

    StreamRead( stream, height_ );
    StreamRead( stream, width_ );
    StreamLine( stream, fontname_ );    // Comic Sans MS
    StreamRead( stream, tempcolor );

    color_ = D3DCOLOR_ARGB( tempcolor.x, tempcolor.y, tempcolor.z, tempcolor.w );

    D3DXCreateFont( GRAPHICS->pDevice, height_, width_, FW_BOLD, 0, FALSE, DEFAULT_CHARSET,
      OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontname_.c_str(), &font_ );
  }

  void Text::Draw( void )
  {
    Vec2 pos = GRAPHICS->WorldToScreenSpace( transform->Position );

    RECT rect;
    rect.left = pos.x;
    rect.top  = pos.y;

    font_->DrawText( NULL, "Hello Wolrd!", -1, &rect, DT_CALCRECT, color_ );

    Drawer::Instance.DrawRectangle( pos, rect.right - rect.left, rect.top - rect.bottom );
    font_->DrawText( NULL, "Hello World!", -1, &rect, 0, color_ );
  }

}   // namespace Framework


