#ifndef SCENE_H
#define SCENE_H
#pragma once

#include <vector>
#include "drawableobject.h"

class Scenes
{
private:
	Scenes();
	~Scenes();
public:
	static Scenes& GetInstance();
private:
	std::vector<DrawableObject> objects;
};

#endif