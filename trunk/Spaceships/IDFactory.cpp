#include "precompiled.h"


int IDFactory::currentID = 0;

IDFactory::IDFactory(void)
{
}

IDFactory::~IDFactory(void)
{
}

int IDFactory::GetNextID()
{
	return IDFactory::currentID++;
}

void IDFactory::ResetID()
{
	ResetID(0);
}

void IDFactory::ResetID(int seed)
{
	IDFactory::currentID = seed;
}

