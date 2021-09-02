#pragma once

#include "GLBases.h"

namespace vox
{
	class UniformBuffer :
		public GLBuffer
	{
	public:
		UniformBuffer() = default;
		UniformBuffer(int size, const void* data, GLuint bufferIndex);

		void setData(const void* data, unsigned size = 0) const;

	private:
	};
}


