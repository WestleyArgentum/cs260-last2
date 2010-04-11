
// Asteroid.h : This is really the bomb.

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
		Vec2 initial_vel_range;
	};

}
