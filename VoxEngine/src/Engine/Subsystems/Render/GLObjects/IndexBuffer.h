#pragma once

#include "GLBases.h"

namespace vox
{
	class IndexBuffer :
		public GLBuffer
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(unsigned size, const void* data);

		void bind() const;
		void unbind() const;

		void setData(const void* data, unsigned size = 0);
	private:
	};
}

