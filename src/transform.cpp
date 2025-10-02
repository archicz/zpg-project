#include "transform.h"

// Translate
TranslateTransform::TranslateTransform(glm::vec3 translate):
	translateMat(1.f)
{
	translateMat = glm::translate(translateMat, translate);
}

glm::mat4 TranslateTransform::GetMat() const
{
	return translateMat;
}


// Rotate
RotateTransform::RotateTransform(float pitch, float yaw, float roll):
	rotateMat(1.f)
{
	rotateMat = glm::rotate(rotateMat, pitch, glm::vec3(1.f, 0.f, 0.f));
	rotateMat = glm::rotate(rotateMat, yaw, glm::vec3(0.f, 1.f, 0.f));
	rotateMat = glm::rotate(rotateMat, roll, glm::vec3(0.f, 0.f, 1.f));
}

glm::mat4 RotateTransform::GetMat() const
{
	return rotateMat;
}

// Scale
ScaleTransform::ScaleTransform(glm::vec3 scale):
	scaleMat(1.f)
{
	scaleMat = glm::scale(scaleMat, scale);
}

glm::mat4 ScaleTransform::GetMat() const
{
	return scaleMat;
}


void Transform::AddChild(TransformationPtr child)
{
	children.push_back(std::move(child));
}

void Transform::Clear()
{
	children.clear();
}

void Transform::Translate(glm::vec3 translate)
{
	auto transformation = std::make_shared<TranslateTransform>(translate);
	AddChild(transformation);
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	auto transformation = std::make_shared<RotateTransform>(pitch, yaw, roll);
	AddChild(transformation);
}

void Transform::Scale(glm::vec3 scale)
{
	auto transformation = std::make_shared<ScaleTransform>(scale);
	AddChild(transformation);
}

glm::mat4 Transform::GetMat() const
{
	glm::mat4 product(1.f);

	for (const auto& child : children)
	{
		product = product * child->GetMat();
	}
	
	return product;
}
