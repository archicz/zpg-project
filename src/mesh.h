#ifndef MESH_H
#define MESH_H
#pragma once

#include <vector>
#include <memory>
#include <glad/glad.h>
#include "vertex.h"

class Mesh
{
public:
	Mesh() = delete;
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
	Mesh(const std::vector<Vertex>& vertices);
	~Mesh();
public:
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh(Mesh&& other) noexcept = delete;
	Mesh& operator=(Mesh&& other) noexcept = delete;
private:
	void BuildVAO();
public:
	bool IsValid() const;
	void Draw() const;
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	GLuint vboId = 0;
	GLuint eboId = 0;
	GLuint vaoId = 0;
};

using MeshPtr = std::shared_ptr<Mesh>;

#endif