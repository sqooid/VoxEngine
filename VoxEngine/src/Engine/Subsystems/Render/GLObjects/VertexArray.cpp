#include "voxpch.h"
#include "VertexArray.h"

namespace vox
{
	static int getTypeSize(const int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:
			return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:
			return sizeof(GLubyte);
		case GL_BYTE:
			return sizeof(GLbyte);
		default:
			assert(!"Unimplemented type used in vertex attribute pointers\n");
		}
	}

	VertexArray::VertexArray()
		: m_Stride(0), m_VertexCount(0)
	{
		glGenVertexArrays(1, &m_Id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_Id);
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_Id);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	unsigned int VertexArray::getCount() const
	{
		return m_VertexCount;
	}

	unsigned VertexArray::getId()
	{
		return m_Id;
	}

	template<>
	void VertexArray::pushAttribute<float>(int count, bool normalize)
	{
		m_Attributes.push_back({ count, GL_FLOAT, normalize });
		m_Stride += count * getTypeSize(GL_FLOAT);
	}

	template<>
	void VertexArray::pushAttribute<unsigned int>(int count, bool normalize)
	{
		m_Attributes.push_back({ count, GL_UNSIGNED_INT, normalize });
		m_Stride += count * getTypeSize(GL_UNSIGNED_INT);
	}

	template<>
	void VertexArray::pushAttribute<unsigned char>(int count, bool normalize)
	{
		m_Attributes.push_back({ count, GL_UNSIGNED_BYTE, normalize });
		m_Stride += count * getTypeSize(GL_UNSIGNED_BYTE);
	}

	template<>
	void VertexArray::pushAttribute<signed char>(int count, bool normalize)
	{
		m_Attributes.push_back({ count, GL_BYTE, normalize });
		m_Stride += count * getTypeSize(GL_BYTE);
	}

	void VertexArray::bindBuffer(const ArrayBuffer& buffer)
	{
		bind();
		buffer.bind();

		int offset = 0;
		for (int i = 0; i < m_Attributes.size(); ++i)
		{
			VertexAttribute& attrib = m_Attributes[i];
			glVertexAttribPointer(i, attrib.size, attrib.type, attrib.normalized, m_Stride, (void*)offset);
			glEnableVertexAttribArray(i);
			offset += attrib.size * getTypeSize(attrib.type);
		}

		m_VertexCount = buffer.getSize() / offset;

		buffer.unbind();
		unbind();
	}

	void VertexArray::bindIndexBuffer(const IndexBuffer& buffer)
	{
		bind();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.getId());
		unbind();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}


