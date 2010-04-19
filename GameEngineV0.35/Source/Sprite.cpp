///////////////////////////////////////////////////////////////////////////////////////
//
//	Sprite.cpp
//	
//	Authors: Benjamin Ellinger, Chris Peters
//	Copyright 2009, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "Sprite.h"
#include "Transform.h"
#include "VertexTypes.h"
#include "Graphics.h"

#include "Core.h"
#include "GameStateManager.h"
#include "WindowsSystem.h"
#include <algorithm>

namespace Framework
{

	Sprite::Sprite()
	{
		transform = NULL;
		Color = Vec4(0,0,0,1);
		pTexture = NULL;
	}

	Sprite::~Sprite()
	{
    GRAPHICS->SpriteList.remove( this );
	}

	void Sprite::Initialize()
	{ 
		//transform = (Transform*)Base->GetComponent( "Transform" ); 
		transform = GetOwner()->has( Transform );
		GRAPHICS->SpriteList.push_back( this );
		pTexture = GRAPHICS->GetTexture( SpriteName );
	}

	void Sprite::Serialize(ISerializer& stream)
	{
    std::string shadername;
		StreamRead(stream, shadername);

    sIndex_ = GRAPHICS->GetShaderIndex( shadername );

		StreamRead(stream, SpriteName);
		StreamRead(stream, Size);
    StreamRead(stream, ZValue );
		StreamRead(stream, Color);

    std::transform( SpriteName.begin(), SpriteName.end(), SpriteName.begin(), tolower );
	}

	void Sprite::Draw( IDirect3DDevice9 *pDevice, ID3DXEffect *shader )
	{
		//Transform the sprite.
		Mat4 matSprite, matTranslate, matRotate;
		//First, scale it to the proper width and height.
		D3DXMatrixScaling(&matSprite, Size.x, Size.y, 1.0f);
		//Then, rotate it to the proper angle.
		D3DXMatrixRotationZ(&matRotate, transform->Rotation);
		D3DXMatrixMultiply(&matSprite, &matSprite, &matRotate);
		//Finally, move it to the proper location (note the floor functions on the positions).
		D3DXMatrixTranslation(&matTranslate, floor(transform->Position.x), floor(transform->Position.y), 0.0f);
		D3DXMatrixMultiply(&matSprite, &matSprite, &matTranslate);

    Graphics::ShaderInfo *shaderInfo = GRAPHICS->GetShaderInfo( sIndex_ );

		//Override the normal world transform and use the transform for this sprite.
		shader->SetTechnique("Technique0");
		pDevice->SetStreamSource(0, GRAPHICS->pQuadVertexBuffer, 0, sizeof(Vertex2D));
		pDevice->SetFVF(VERTEX2D_FVF);

		Mat4 worldViewProj = matSprite * GRAPHICS->ViewProjMatrix;
		UINT numberOfPasses = 0;
		shader->SetMatrix( "WorldViewProj", &worldViewProj );
		shader->Begin(&numberOfPasses,0);
		shader->SetTexture( "texture0" , pTexture );
		shader->SetVector( "color", &Color );

    shader->SetFloat( "fTime", static_cast<float>( CORE->GetTime() ) );

      // Initialize any extra information about this particular shader for the sprite that we are
      //  going to draw.
    if ( shaderInfo->data_ )
    {
      shaderInfo->data_->InitPhase( shader, this );
    }

		for ( UINT pass = 0; pass < numberOfPasses; ++pass )
		{
			shader->BeginPass(pass);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			shader->EndPass();
		}

		shader->End();
	}

}

