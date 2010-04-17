
#include <string>

#include "Component.h"
#include "VMath.h"

namespace Framework
{

	GOC * CreateObjectAt( Vec2& position, float rotation, const std::string& file );
	void SpawnRandomAsteroids( void );

}