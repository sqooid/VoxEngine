#pragma once

#include "GLBases.h"

namespace vox
{
	//////////////////////////////////////////////////////////////////////////
	// Array Buffer Class ////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class ArrayBuffer
		: public GLBuffer
	{
	public:
		ArrayBuffer() = default;
		ArrayBuffer(unsigned size, const void* data, BufferUsage usage);

		~ArrayBuffer();

		template<typename T>
		ArrayBuffer(const std::vector<T>& data, BufferUsage usage);

		void bind() const;
		void unbind() const;

		void setData(const void* data, unsigned size = 0);
	private:
		unsigned int m_Usage;
	};

	template<typename T>
	ArrayBuffer::ArrayBuffer(const std::vector<T>& data, BufferUsage usage)
	{
		m_DataSize = data.size() * sizeof(T);
		m_Usage = getUsage(usage);

		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, m_DataSize, &data[0], m_Usage);
	}

}





