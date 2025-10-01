#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H
#pragma once

#include <memory>
#include "mesh.h"
#include "materials.h"
#include "transform.h"

using MaterialPtr = std::shared_ptr<Material>;
using MeshPtr = std::shared_ptr<Mesh>;

class DrawableObject
{
public:
	DrawableObject(MaterialPtr mat, MeshPtr msh);
	~DrawableObject();
public:
	void Draw();
public:
	Transform& GetTransform();
private:
	MeshPtr mesh;
	Transform modelMat;
	MaterialPtr material;
};

#endif