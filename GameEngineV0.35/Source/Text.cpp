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
    GRAPHICS->TextList.erase( this );
  }

  void Text::Initialize( void )
  {
    transform = GetOwner()->has( Transform );

    GRAPHICS->TextList.push_back( this );
  }

  void Text::Serialize( ISerializer &stream )
  {
    int red, green, blue, alpha;

    StreamRead( stream, height_ );
    StreamRead( stream, width_ );
    StreamLine( stream, fontname_ );    // Comic Sans MS

      // Read in the color of the texture!
    StreamRead( stream, red );
    StreamRead( stream, green );
    StreamRead( stream, blue );
    StreamRead( stream, alpha );

    SetColor( red, green, blue, alpha );

    pFont_ = GRAPHICS->GetFont( fontname_, width_, height_ );
  }

  void Text::SetColor( char red, char green, char blue, char alpha )
  {
    color_ = D3DCOLOR_ARGB( alpha, red, green, blue );
  }

  void Text::Draw( void )
  {
    Vec2 pos = GRAPHICS->WorldToScreenSpace( transform->Position );

    RECT rect;
    rect.left = pos.x;
    rect.top  = pos.y;

    pFont_->DrawText( NULL, "Hello Wolrd!", -1, &rect, DT_CALCRECT, color_ );

    Drawer::Instance.DrawRectangle( pos, rect.right - rect.left, rect.top - rect.bottom );
    pFont_->DrawText( NULL, "Hello World!", -1, &rect, 0, color_ );
  }

}   // namespace Framework


