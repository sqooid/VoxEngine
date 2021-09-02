#include "voxpch.h"
#include "IndexBuffer.h"

namespace vox
{

	IndexBuffer::IndexBuffer(unsigned size, const void* data)
	{
		m_DataSize = size;
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		unbind();
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getId());
	}

	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::setData(const void* data, unsigned size /*= 0*/)
	{
		bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size ? size : getSize(), data);
		unbind();
	}
}