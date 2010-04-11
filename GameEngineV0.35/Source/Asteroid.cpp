
// Asteroid.cpp : This is really a bomb.

#include "Precompiled.h"
#include "Asteroid.h"
#include "Transform.h"
#include "Factory.h"
#include "Body.h"
#include "PRNG.h"

namespace Framework
{
	void Asteroid::Initialize()
	{
		SpawnTime = timeGetTime();

		// go!
		Body * bod = GetOwner()->has(Body);
		bod->SetVelocity(Vec2(Utils::Random(-initial_vel_range.x, initial_vel_range.x), Utils::Random(-initial_vel_range.y, initial_vel_range.y)));
	}

	void Asteroid::Serialize(ISerializer& stream)
	{
		StreamRead(stream,Fuse);
		StreamRead(stream,SubSpawnCount);
		StreamRead(stream, initial_vel_range);
	}

	void Asteroid::SendMessage(Message* m)
	{
		if( m->MessageId == Mid::Collide )
		{			
			if( (int)timeGetTime() - SpawnTime > Fuse )
			{
				GetOwner()->Destroy();
				if( SubSpawnCount > 0 )
				{			
					Transform * transform = GetOwner()->has(Transform);
					for(int i=-1;i<=1;++i)
					{
						Vec2 dir( sin( float(i)*D3DX_PI*0.3f) , cos( float(i)*D3DX_PI*0.3f) );		
						GOC * a = FACTORY->Create("Objects\\Shrapnel.txt");
						Body * bodyA = a->has(Body);
						bodyA->SetVelocity(dir * 120);
						bodyA->SetPosition(transform->Position);
					}
				}
			}
		}
	}

}