#include "pch.h"
#include "Buffer.h"



/////////////////////////////////////////////////////////
//VBO////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

VBO::VBO(const void* data, std::size_t size) : count(-1)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VBO::VBO(const void* data, const std::size_t size, const int vertexCount)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	count = vertexCount;
}

VBO::~VBO()
{
	glDeleteBuffers(1, &id);
}


void VBO::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/////////////////////////////////////////////////////////
//Layout////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

void VertexLayout::Enable() const
{
	unsigned offset = 0;
	for (unsigned i = 0; i < attribs.size(); ++i)
	{
		const auto& at = attribs[i];
		glVertexAttribPointer(i, at.count, at.type, at.normalized, stride, (void*)offset);
		glEnableVertexAttribArray(i);
		offset += at.count * at.GetTypeSize();
	}
}


/////////////////////////////////////////////////////////
//VAO////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

VAO::VAO()
{
	glGenVertexArrays(1, &id);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &id);
}

void VAO::AddBuffer(const VBO& vbo)
{
	Bind();
	vbo.Bind();
	vbo.GetLayout().Enable();
	count = vbo.Count();
}

void VAO::Bind() const
{
	glBindVertexArray(id);
}

void VAO::Unbind() const
{
	glBindVertexArray(0);
}




/////////////////////////////////////////////////////////
//EBO////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

EBO::EBO(const unsigned int* data, unsigned int count) : count(count)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

EBO::~EBO()
{
    glDeleteBuffers(1, &id);
}


void EBO::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

