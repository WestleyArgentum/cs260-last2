#include "Model.h"

Model::Model(void)
{
}

Model::~Model(void)
{	
}

void Model::SetCurrentController(Controller* value)
{
	controller = value;
}