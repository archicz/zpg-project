#ifndef TRANSFORM_H
#define TRANSFORM_H
#pragma once

#include <memory>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ITransformation
{
public:
	virtual ~ITransformation() = default;
	virtual glm::mat4 GetMat() const = 0;
};

using TransformationPtr = std::shared_ptr<ITransformation>;

// Translate
class TranslateTransform : public ITransformation 
{
public:
	TranslateTransform(glm::vec3 translate);
public:
	virtual glm::mat4 GetMat() const override;
private:
	glm::mat4 translateMat;
};

// Rotate
class RotateTransform : public ITransformation 
{
public:
	RotateTransform(float pitch, float yaw, float roll);
public:
	virtual glm::mat4 GetMat() const override;
private:
	glm::mat4 rotateMat;
};

// Scale
class ScaleTransform : public ITransformation 
{
public:
	ScaleTransform(glm::vec3 scale);
public:
	virtual glm::mat4 GetMat() const override;
private:
	glm::mat4 scaleMat;
};

class Transform : public ITransformation
{
public:
	Transform() = default;
	~Transform() = default;
public:
	Transform(const Transform&) = delete;
	Transform& operator=(const Transform&) = delete;
	Transform(Transform&&) = default;
	Transform& operator=(Transform&&) = default;
public:
	void AddChild(TransformationPtr child);
	void Clear();
public:
	void Translate(glm::vec3 translate);
	void Rotate(float pitch, float yaw, float roll);
	void Scale(glm::vec3 scale);
public:
	virtual glm::mat4 GetMat() const override;
private:
	std::vector<TransformationPtr> children;
};

#endif