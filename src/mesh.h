#ifndef MESH_H
#define MESH_H
#pragma once

#include <vector>
#include <glad/glad.h>
#include "vertex.h"
#include "materialsystem.h"

class Mesh
{
public:
	Mesh() = delete;
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Material>& materials);
	~Mesh();
public:
	bool IsValid() const;
	void Draw() const;
public:
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh(Mesh&& other) noexcept = delete;
	Mesh& operator=(Mesh&& other) noexcept = delete;
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Material> materials;

	GLuint vboId = 0;
	GLuint eboId = 0;
	GLuint vaoId = 0;
};

#endif