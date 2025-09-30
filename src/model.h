#ifndef MODEL_H
#define MODEL_H
#pragma once

#include <vector>
#include <unordered_map>
#include <glad/glad.h>

class Model
{
public:
	Model();
	~Model();
public:
	void CreateFromTriangles(const float* vtx, uint32_t numVtx);
	inline void CreateFromTriangles(const std::vector<float>& vtx)
	{
		CreateFromTriangles(vtx.data(), vtx.size());
	}
public:
	bool IsValid() const;
	void Draw();
private:
	GLuint numVertices;
	GLuint vboId = 0;
	GLuint vaoId = 0;
};

#endif