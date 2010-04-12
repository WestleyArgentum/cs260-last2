
// Asteroid.h : This is really the bomb.

#ifndef ASTEROIDCONTROLLER
#define ASTEROIDCONTROLLER

#include "Component.h"
#include "VMath.h"

namespace Framework
{

	///Sample Demo Component Explosive Bomb. Explodes on contact after
	///its fuse has counted down.
	class Asteroid : public GameComponent
	{
	public:
		int Fuse;
		int SubSpawnCount;
		int SpawnTime;

		virtual void Initialize();
		virtual void Serialize(ISerializer& stream);
		virtual void SendMessage(Message* m);

	private:
		int vel_range_x;
		int vel_range_y;
	};

}

#endif
