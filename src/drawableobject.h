#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H
#pragma once

#include <memory>
#include "model.h"
#include "materialsystem.h"
#include "transform.h"

using MaterialPtr = std::shared_ptr<Material>;
using ModelPtr = std::shared_ptr<Model>;

class DrawableObject
{
public:
	DrawableObject(MaterialPtr mat, ModelPtr mdl);
	~DrawableObject();
public:
	void Draw();
public:
	Transform& GetTransform();
private:
	ModelPtr model;
	Transform modelMat;
	MaterialPtr material;
};

#endif