#pragma once

namespace vox
{
	struct GLObject
	{
		[[nodiscard]] GLuint getId() const { return m_Id; }
		GLuint m_Id = 0;
	};

	enum class BufferUsage
	{
		Static,
		Dynamic,
	};

	inline unsigned getUsage(BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::Static:
			return GL_STATIC_DRAW;
			break;
		case BufferUsage::Dynamic:
			return GL_DYNAMIC_DRAW;
			break;
		default:
			assert(!"Unimplemented buffer usage used\n");
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Base class for all GL buffer objects //////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	struct GLBuffer : GLObject
	{
		GLBuffer() { glGenBuffers(1, &m_Id); }
		~GLBuffer() { glDeleteBuffers(1, &m_Id); }

		[[nodiscard]] GLuint getSize() const { return m_DataSize; }
		GLuint m_DataSize = 0;
	};
}
