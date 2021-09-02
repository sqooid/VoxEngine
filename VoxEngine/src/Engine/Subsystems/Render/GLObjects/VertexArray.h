#pragma once

#include "ArrayBuffer.h"
#include "IndexBuffer.h"

namespace vox
{
	struct VertexAttribute
	{
		int size;
		unsigned int type;
		int8_t normalized;
	};

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void bind() const;
		void unbind() const;

		unsigned int getCount() const;
		unsigned getId();

		template <typename T>
		void pushAttribute(int count, bool normalize);

		void bindBuffer(const ArrayBuffer& buffer);

		void bindIndexBuffer(const IndexBuffer& buffer);

	private:
		unsigned int m_Id;
		unsigned int m_VertexCount;

		std::vector<VertexAttribute> m_Attributes;
		int m_Stride;
	};
}





