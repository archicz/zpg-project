#ifndef VERTEX_H
#define VERTEX_H
#pragma once

#include <array>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex
{
	glm::vec3 coords;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

struct VertexAttribute
{
	GLuint elementSize;
	GLint numElements;
	GLenum dataType;
	GLboolean normalized;
};

constexpr VertexAttribute VertexAttributes[] =
{
	VertexAttribute{ sizeof(float), 3, GL_FLOAT, GL_FALSE },	// Coords
	VertexAttribute{ sizeof(float), 3, GL_FLOAT, GL_FALSE },	// Normal
	VertexAttribute{ sizeof(float), 2, GL_FLOAT, GL_FALSE } 	// Texture Coords
};

#endif