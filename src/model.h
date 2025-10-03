#ifndef MODEL_H
#define MODEL_H
#pragma once

#include "asset.h"
#include "mesh.h"

class Model
{
public:
	Model();
	~Model();
};

using ModelPtr = std::shared_ptr<Model>;

#endif