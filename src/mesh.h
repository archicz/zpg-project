#ifndef MESH_H
#define MESH_H
#pragma once

#include <vector>
#include <memory>
#include <glad/glad.h>
#include "vertex.h"

struct MeshPart
{
	size_t numIndices;
	size_t baseIndex;
	GLint baseVertex;
};

class Mesh
{
public:
	Mesh() = delete;
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<MeshPart>& parts);
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
	Mesh(const std::vector<Vertex>& vertices);
	~Mesh();
public:
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh(Mesh&& other) noexcept = delete;
	Mesh& operator=(Mesh&& other) noexcept = delete;
private:
	void BuildVBO(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
	void BuildVAO();
public:
	void Destroy();
	bool IsValid() const;
public:
	void DrawPart(size_t partIndex);
	void DrawFull() const;
private:
	std::vector<MeshPart> parts;
	GLuint vboId = 0;
	GLuint eboId = 0;
	GLuint vaoId = 0;
};

using MeshPtr = std::shared_ptr<Mesh>;

#endif