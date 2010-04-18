///////////////////////////////////////////////////////////////////////////////////////
///
///	\file VMath.h
///	Typedefs the DirectX Extension math library and provides some utility functions.
///
///	Authors: Chris Peters, Benjamin Ellinger
///	Copyright 2009, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#pragma once //Makes sure this header is only included once

//Include our math headers
#include <d3dx9.h>
#include <cmath>
#include "Serialization.h"
#include "DataStream.h"

typedef D3DXVECTOR2   Vec2;
typedef D3DXVECTOR3   Vec3;
typedef D3DXVECTOR4   Vec4;
typedef D3DXMATRIXA16 Mat4;
typedef D3DXCOLOR     Color;

namespace Framework
{
	//Extended serialization operators of compound math types.
	inline void StreamRead(ISerializer& stream,Vec2& v)
	{
		StreamRead(stream,v.x);
		StreamRead(stream,v.y);
	}

	inline void StreamRead(ISerializer& stream,Vec3& v)
	{
		StreamRead(stream,v.x);
		StreamRead(stream,v.y);
		StreamRead(stream,v.z);
	}

	inline void StreamRead(ISerializer& stream,Vec4& v)
	{
		StreamRead(stream,v.x);
		StreamRead(stream,v.y);
		StreamRead(stream,v.z);
		StreamRead(stream,v.w);
	}

  // For DataStreams too!
	inline void StreamRead(DataStream &stream,Vec2& v)
	{
		StreamRead(stream,v.x);
		StreamRead(stream,v.y);
	}

	inline void StreamRead(DataStream &stream,Vec3& v)
	{
		StreamRead(stream,v.x);
		StreamRead(stream,v.y);
		StreamRead(stream,v.z);
	}

	inline void StreamRead(DataStream &stream,Vec4& v)
	{
		StreamRead(stream,v.x);
		StreamRead(stream,v.y);
		StreamRead(stream,v.z);
		StreamRead(stream,v.w);
	}

  // Color
	inline void StreamRead( DataStream &stream, Color &c )
	{
		StreamRead(stream, c.r);
		StreamRead(stream, c.g);
		StreamRead(stream, c.b);
		StreamRead(stream, c.a);
	}

  // Writing the information into DataStreams now!
	inline void StreamWrite(DataStream &stream, const Vec2 &v)
	{
		StreamWrite(stream,v.x);
		StreamWrite(stream,v.y);
	}

	inline void StreamWrite(DataStream &stream, const Vec3 &v)
	{
		StreamWrite(stream,v.x);
		StreamWrite(stream,v.y);
		StreamWrite(stream,v.z);
	}

	inline void StreamWrite(DataStream &stream, const Vec4 &v)
	{
		StreamWrite(stream,v.x);
		StreamWrite(stream,v.y);
		StreamWrite(stream,v.z);
		StreamWrite(stream,v.w);
	}

	inline void StreamWrite( DataStream &stream, const Color &c )
	{
		StreamWrite(stream, c.r);
		StreamWrite(stream, c.g);
		StreamWrite(stream, c.b);
		StreamWrite(stream, c.a);
	}


	inline float Dot(const Vec2& a, const Vec2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline float LengthSquared(const Vec2& a)
	{
		return a.x * a.x + a.y * a.y;
	}

	inline float Normalize(Vec2& a)
	{
		float len = sqrt( LengthSquared(a) );	
		a.x /= len;
		a.y /= len;
		return len;
	}

	template< typename RefType >
	void SafeRelease( RefType& interfacePtr )
	{
		if( interfacePtr ) interfacePtr->Release();
		interfacePtr = NULL;
	}

	template< typename RefType >
	void SafeDelete( RefType& objectPtr )
	{
		if( objectPtr ) delete objectPtr;
		objectPtr = NULL;
	}

  inline Color Convert( const Vec4 &vec )
  {
    return Color( vec.x * 255.0f, vec.y * 255.0f, vec.z * 255.0f, vec.w * 255.0f);
  }

	typedef unsigned int uint;
}
