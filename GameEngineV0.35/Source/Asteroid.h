
// Asteroid.h : This is really the bomb.

#ifndef ASTEROIDCONTROLLER
#define ASTEROIDCONTROLLER

#include "Component.h"
#include "VMath.h"
#include "Body.h"
#include "Transform.h"
#include "IController.h"

namespace Framework
{

	///Sample Demo Component Explosive Bomb. Explodes on contact after
	///its fuse has counted down.
	class Asteroid : public Controller
	{
	public:
		int Fuse;
		int SubSpawnCount;
		int SpawnTime;

		virtual void Initialize();
    virtual void LogicalUpdate( float dt );
		virtual void Serialize(ISerializer& stream);
		virtual void SendMessage(Message* m);

	private:
		int vel_range_x;
		int vel_range_y;

    Transform *transform;
    Body *body;
	};

}

#endif
