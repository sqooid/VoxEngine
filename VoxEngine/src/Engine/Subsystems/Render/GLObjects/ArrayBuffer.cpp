#include "voxpch.h"
#include "ArrayBuffer.h"

namespace vox
{
	ArrayBuffer::ArrayBuffer(unsigned size, const void* data, BufferUsage usage)
	{
		m_DataSize = size;
		m_Usage = getUsage(usage);

		bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, m_Usage);
		unbind();
	}

	ArrayBuffer::~ArrayBuffer()
	{
	}

	void ArrayBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, getId());
	}

	void ArrayBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void ArrayBuffer::setData(const void* data, unsigned size /*= 0*/)
	{
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size ? size : getSize(), data);
		unbind();
	}

}