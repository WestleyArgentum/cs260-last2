///////////////////////////////////////////////////////////////////////////////
///
///	\file Graphics.h
/// Defines grahics system.
///	
///	Authors: Benjamin Ellinger, Chris Peters
///	Copyright 2009, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once //Makes sure this header is only included once

#include "Engine.h"
#include "Sprite.h"
#include "Text.h"

namespace Framework
{	
	//Forward Declaration of Graphics Objects
	class Camera;

	///A two-dimensional hardware accelerated non fixed function 
	///sprite based graphics system.
	///Provides Sprite and Camera GameComponents.
	class Graphics : public ISystem
	{
	public:
		///Update by rendering the scene
		void Update(float dt);
		virtual std::string GetName(){return "Graphics";}

    //Initialize the Direct3D system.
		Graphics();
		~Graphics();

		//Get a texture asset. Will return null if texture is not loaded
    PixelShaders GetShaderIndex( const std::string &shadername );
		IDirect3DTexture9* GetTexture( const std::string &texture );
    ID3DXFont* GetFont( const std::string &fontname, unsigned width, unsigned height );

		Vec2 ScreenToWorldSpace(Vec2);
    Vec2 WorldToScreenSpace(Vec2);
		void SetWindwProperties(HWND hWnd,int screenWidth,int screenHeight);

  private:
    struct Font
    {
      Font( void ) : width_(0), height_(0) {;}

        // Used to sort Fonts based off internal members.
      bool operator< ( const Font &rhs ) const;
      bool operator== ( const Font &rhs ) const;

      std::string name_;
      unsigned width_;
      unsigned height_;
    };    // Font

	private:
		void Initialize();
		//Create a vertex buffer for our sprites.
		bool InitGeometry();	
		//Load all the textures for our game.
		void LoadAssets();		
		//Load an individual texture.
		void LoadTexture(const std::string& filename);
		//Load an individual font.
	  void LoadFont( const std::string &fontname, unsigned width, unsigned height );
		//Set up the default world, view, and projection matrices
		void SetupMatrices();
		//Load a effect file
		bool LoadEffect( PixelShaders index, const std::string &filename, const std::string &shadername );

  // Loads the effect in a way that the PixelShader enum and the Shader.fx are named the same
#define LOAD_EFFECT( SHADER ) LoadEffect( SHADER, "Shaders/" #SHADER ".fx", #SHADER )

		//Draw Debug Data
		void DrawDebugInfo();
		//Draw the world
		void DrawWorld( float dt );
		//TODO: Need to handle device lost / device reset
		void DeviceLost();
		void DeviceReset();

	public:
		//The active camera
		Camera *CurrentCamera;
		HWND HWnd;
		int ScreenWidth;
		int ScreenHeight;

		//Direct3D
		IDirect3D9*				pD3D;
		//Direct3D Device
		IDirect3DDevice9*		pDevice;

		//The quad vertex buffer
		IDirect3DVertexBuffer9*	pQuadVertexBuffer;

		//Stored texture assets
		typedef std::map<std::string, IDirect3DTexture9*> TextureMap;
    typedef std::map<Font, ID3DXFont*>                FontMap;
		TextureMap Textures;
    FontMap Fonts;

    typedef std::map< std::string, PixelShaders >     ShaderIndex;
    ShaderIndex shadermap_;

		//An array for our pixel shaders.
		ID3DXEffect* Shaders[NumberOfShaders];		
		
		//World Projection and view matrices
		Mat4 ProjMatrix;
		Mat4 ViewMatrix;
		Mat4 ViewProjMatrix;

		//Stored copy of the DirectX Presentation Parameters
		D3DPRESENT_PARAMETERS PresentParameters;
		Vec2 SurfaceSize;
		ObjectLinkList<Sprite> SpriteList;
    ObjectLinkList<Text> TextList;
	};

	//A global pointer to the Graphics system, used to access it anywhere.
	extern Graphics* GRAPHICS;
}

