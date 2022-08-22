#include "pch.h"

#include "Buffer.h"
#include "glad/glad.h"

namespace Crave
{
	VBO::VBO(const void* data, std::size_t size) : m_Count(-1)
	{
		glGenBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VBO::VBO(const void* data, const std::size_t size, const int vertexCount)
	{
		glGenBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		m_Count = vertexCount;
	}

	VBO::~VBO()
	{
		glDeleteBuffers(1, &m_Id);
	}


	void VBO::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	}

	void VBO::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



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
		: m_Stride(0), m_Attribs(list)
	{
		for (auto& attrib : m_Attribs)
		{
			m_Stride += attrib.count * sizeof(attrib.GetTypeSize());
		}
	}

	void VertexLayout::Enable() const
	{
		unsigned offset = 0;
		for (unsigned i = 0; i < m_Attribs.size(); ++i)
		{
			const auto& at = m_Attribs[i];

			if (at.type == GL_FLOAT)
				glVertexAttribPointer(i, at.count, at.type, at.normalized, m_Stride, (void*)offset);
			else if (at.type == GL_INT)
				glVertexAttribIPointer(i, at.count, at.type, m_Stride, (void*)offset);
			else
				ASSERT(0, "Unsupported type");

			glEnableVertexAttribArray(i);
			offset += at.count * at.GetTypeSize();
		}
	}



	EBO::EBO(const unsigned int* data, unsigned int count) : m_Count(count)
	{
		glGenBuffers(1, &m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	EBO::~EBO()
	{
		glDeleteBuffers(1, &m_Id);
	}


	void EBO::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	}

	void EBO::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}



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
}