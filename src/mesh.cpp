#include <plog/Log.h>
#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLuint>& _indices, const std::vector<Material>& _materials):
	vertices(std::move(_vertices)), indices(std::move(_indices)), materials(std::move(_materials))
{
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &eboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);

	GLchar* vertexPtr = nullptr;
	GLuint numVertexAttributes = sizeof(VertexAttributes) / sizeof(VertexAttribute);
	
	for (int i = 0; i < numVertexAttributes; i++)
	{
		const auto& attr = VertexAttributes[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attr.numElements, attr.dataType, GL_FALSE, sizeof(Vertex), static_cast<GLvoid*>(vertexPtr));

		vertexPtr += attr.elementSize * attr.numElements;
	}
}

Mesh::~Mesh()
{
}

bool Mesh::IsValid() const
{
	return (vboId != 0 && eboId != 0 && vaoId != 0);
}

void Mesh::Draw() const
{
	if (IsValid())
	{
		glBindVertexArray(vaoId);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
}