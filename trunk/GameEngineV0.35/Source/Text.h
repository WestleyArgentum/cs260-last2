#pragma once

#include "Composition.h"
#include "VMath.h"

namespace Framework
{
	class Transform;

  class Text : public GameComponent
  {
    public:
      Text *Next;
      Text *Prev;

      Text( void );
      ~Text( void );

		  virtual void Serialize( ISerializer &stream );	
		  virtual void Initialize( void );

      void ChangeFont( unsigned width, unsigned height, const std::string &fontname );

        /// Sets the color of the text on the screen. Maybe be helpful to have specific function for
        ///  fading colors if desired.
      void SetColor( unsigned char red, unsigned char green, unsigned char blue,
        unsigned char alpha );

        /// Sets the text displayed to the string given. (printf style, note, newlines may not work) 
      void SetText( const char *msg, ... );

        /// Graphics calls this to draw the text object
      void Draw( void );

    public:
      ID3DXFont *pFont_;      ///< Pointer to the font that this text component draws in
      unsigned height_;       ///< The height of the text font
      unsigned width_;        ///< The width of this font
      std::string fontname_;  ///< The name of the font that we are drawing with!
      D3DCOLOR color_;        ///< Color of the font we are drawing.

      std::string text_;      ///< Holds the text that we display to the screen
      bool screen_;           ///< States if the text should be displayed in screen or world coords

      Transform *transform;   ///< Holds a pointer to the transform component of the game object
  };    // Text

}   // namespace Framework

