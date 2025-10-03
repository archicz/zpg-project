#include <plog/Log.h>
#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<MeshPart>& _parts):
	parts(_parts)
{
	BuildVBO(vertices, indices);
	BuildVAO();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	BuildVBO(vertices, indices);
	BuildVAO();

	if (eboId != 0)
	{
		parts.push_back(MeshPart{ 0, indices.size(), 0 });
	}
	else
	{
		parts.push_back(MeshPart{ 0, vertices.size(), 0 });
	}
}

Mesh::Mesh(const std::vector<Vertex>& vertices)
{
	BuildVBO(vertices, std::vector<GLuint>());
	BuildVAO();

	parts.push_back(MeshPart{ 0, vertices.size(), 0 });
}

Mesh::~Mesh()
{
	Destroy();
}

void Mesh::BuildVBO(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	if (!indices.empty())
	{
		glGenBuffers(1, &eboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	}
}

void Mesh::BuildVAO()
{
	if (vboId == 0)
	{
		return;
	}

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	if (eboId != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	}
	
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

void Mesh::Destroy()
{
	if (IsValid())
	{
		glDeleteVertexArrays(1, &vaoId);
		glDeleteBuffers(1, &vboId);

		if (eboId != 0)
		{
			glDeleteBuffers(1, &eboId);
		}
	}

	vboId = 0;
	eboId = 0;
	vaoId = 0;
}

bool Mesh::IsValid() const
{
	return (vboId != 0 && vaoId != 0);
}

void Mesh::DrawPart(size_t partIndex)
{
	if (!IsValid())
	{
		return;
	}

	glBindVertexArray(vaoId);

	try
	{
		auto part = parts.at(partIndex);

		if (eboId != 0)
		{
			glDrawElementsBaseVertex(
				GL_TRIANGLES,
				part.numIndices,
				GL_UNSIGNED_INT,
				reinterpret_cast<const void*>(sizeof(GLuint) * part.baseIndex),
				part.baseVertex
			);
		}
		else
		{
			glDrawArrays(
				GL_TRIANGLES,
				part.baseIndex,
				part.numIndices
			);
		}
	}
	catch(const std::exception& e)
	{
		PLOGE << e.what();
	}
}

void Mesh::DrawFull() const
{
	if (!IsValid())
	{
		return;
	}

    glBindVertexArray(vaoId);

	if (eboId != 0)
	{
		for (const auto& part : parts)
		{
			glDrawElementsBaseVertex(
				GL_TRIANGLES,
				part.numIndices,
				GL_UNSIGNED_INT,
				reinterpret_cast<const void*>(sizeof(uint32_t) * part.baseIndex),
				part.baseVertex
			);
		}
	}
	else
	{
		for (const auto& part : parts)
		{
			glDrawArrays(
				GL_TRIANGLES,
				part.baseIndex,
				part.numIndices
			);
		}
	}
}