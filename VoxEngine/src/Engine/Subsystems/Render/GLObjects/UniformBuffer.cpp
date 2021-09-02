#include "voxpch.h"
#include "UniformBuffer.h"

namespace vox
{
	UniformBuffer::UniformBuffer(int size, const void* data, GLuint bufferIndex)
	{
		m_DataSize = size;

		glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bufferIndex, m_Id);
	}

	void UniformBuffer::setData(const void* data, unsigned size /*= 0*/) const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, getId());
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size ? size : getSize(), data);
	}
}
