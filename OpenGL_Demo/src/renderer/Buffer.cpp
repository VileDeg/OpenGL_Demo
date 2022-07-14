#include "pch.h"

#include "Buffer.h"
#include "glad/glad.h"

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

const unsigned VertexLayout::VertexAttribute::GetTypeSize() const
{
	switch (type)
	{
	case GL_FLOAT:         return sizeof(GLfloat);
	case GL_UNSIGNED_INT:  return sizeof(GLuint);
	case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
	}

	return 0;
}

VertexLayout::VertexLayout(const std::initializer_list<VertexAttribute>& list)
: stride(0), attribs(list)
{
	for (auto& attrib : attribs)
	{
		stride += attrib.count * sizeof(attrib.GetTypeSize());
	}
}

void VertexLayout::Enable() const
{
	unsigned offset = 0;
	for (unsigned i = 0; i < attribs.size(); ++i)
	{
		const auto& at = attribs[i];

		if      (at.type == GL_FLOAT)
			glVertexAttribPointer(i, at.count, at.type, at.normalized, stride, (void*)offset);
		else if (at.type == GL_INT  )
			glVertexAttribIPointer(i, at.count, at.type, stride, (void*)offset);
		else
			ASSERT(0, "Unsupported type");

		glEnableVertexAttribArray(i);
		offset += at.count * at.GetTypeSize();
	}
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

/////////////////////////////////////////////////////////
//UBO////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

ShaderBlock::ShaderBlock(const char* name, const void* data,
	const std::size_t size, const unsigned type)
	: m_Name(name), m_TypeUInt(static_cast<unsigned>(type))
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(m_TypeUInt, m_Id);
	glBufferData(m_TypeUInt, size, data, GL_STATIC_DRAW);
}

ShaderBlock::~ShaderBlock()
{
	glDeleteBuffers(1, &m_Id);
}


void ShaderBlock::Upload(const void* data, const std::size_t size, const unsigned offset)
{
	glBindBuffer(m_TypeUInt, m_Id);
	glBufferSubData(m_TypeUInt, offset, size, data);
	glBindBuffer(m_TypeUInt, 0);
}

void ShaderBlock::Bind(unsigned bindingPoint)
{
	glBindBufferBase(m_TypeUInt, bindingPoint, m_Id);
}

void ShaderBlock::Unbind()
{
	glBindBuffer(m_TypeUInt, 0);
}