#include <plog/Log.h>
#include "model.h"

struct VertexAttribute
{
	GLuint elementSize;
	GLint numElements;
	GLenum dataType;
	GLboolean normalized;
};

static constexpr std::array<VertexAttribute, 2> VertexAttributes =
{
	VertexAttribute{ sizeof(float), 3, GL_FLOAT, GL_FALSE },	// Coords
	VertexAttribute{ sizeof(float), 3, GL_FLOAT, GL_FALSE }		// Normal
};


Model::Model()
{
}

Model::~Model()
{
}

void Model::CreateFromTriangles(const float* vtx, uint32_t numVtx)
{
	numVertices = numVtx;

	GLuint vertexStride = 0;
	GLchar* vertexPtr = nullptr;

	for (const auto& attr : VertexAttributes)
	{
		vertexStride += attr.elementSize * attr.numElements;
	}

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, numVtx * vertexStride, vtx, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	
	for (int i = 0; i < VertexAttributes.size(); i++)
	{
		const auto& attr = VertexAttributes[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attr.numElements, attr.dataType, GL_FALSE, vertexStride, static_cast<GLvoid*>(vertexPtr));

		vertexPtr += attr.elementSize * attr.numElements;
	}
}

bool Model::IsValid() const
{
	return (vboId != 0);
}

void Model::Draw()
{
	if (IsValid())
	{
		glBindVertexArray(vaoId);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}