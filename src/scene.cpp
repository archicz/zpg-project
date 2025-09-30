#include "scene.h"

Scenes::Scenes()
{
}

Scenes::~Scenes()
{
}

Scenes& Scenes::GetInstance()
{
	static Scenes instance;
	return instance;
}