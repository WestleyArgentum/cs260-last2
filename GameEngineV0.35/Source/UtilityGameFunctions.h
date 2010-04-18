
#include <string>

#include "Component.h"
#include "VMath.h"
#include "Composition.h"

namespace Framework
{

	GOC * CreateObjectAt( Vec2& position, float rotation, const std::string& file );
  GOC * CreateObjectAt(Vec2& position,float rotation,const std::string& file, GOCId id);
	void SpawnRandomAsteroids( void );

}