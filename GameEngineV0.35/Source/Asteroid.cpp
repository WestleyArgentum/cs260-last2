
// Asteroid.cpp : This is really a bomb.

#include "Precompiled.h"
#include "Asteroid.h"
#include "Transform.h"
#include "Factory.h"
#include "Body.h"
#include "PRNG.h"
#include "Physics.h"

namespace Framework
{
	void Asteroid::Initialize()
	{
		SpawnTime = timeGetTime();

		// go!
		Body * bod = GetOwner()->has(Body);

		// give the asteroid a random velocity
		bod->SetVelocity(Vec2(static_cast<float>(Utils::Random(-vel_range_x, vel_range_x)), 
													static_cast<float>(Utils::Random(-vel_range_y, vel_range_y))));

    transform = GetOwner()->has(Transform);
    body = GetOwner()->has(Body);
	}

  void Asteroid::LogicalUpdate( float dt )
  {
    transform->Position = body->Position;
  }

	void Asteroid::Serialize(ISerializer& stream)
	{
		StreamRead(stream,Fuse);
		StreamRead(stream,SubSpawnCount);
		StreamRead(stream, vel_range_x);
		StreamRead(stream, vel_range_y);
	}

	void Asteroid::SendMessage(Message* m)
	{
    MessageCollide *mc = dynamic_cast<MessageCollide*>(m);

		if( m->MessageId == Mid::Collide && mc && !mc->CollidedWith->has(Asteroid))
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