#include "drawableobject.h"

DrawableObject::DrawableObject(MaterialPtr mat, ModelPtr mdl):
	material(mat), model(mdl)
{
}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::Draw()
{
	// shaderProgram->Use();
	// shaderProgram->SetMat4("modelMat", modelMat.GetMat());
	model->Draw();
}

Transform& DrawableObject::GetTransform()
{
	return modelMat;
}
