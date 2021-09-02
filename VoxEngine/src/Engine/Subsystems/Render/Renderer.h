#pragma once

#include "../../Subsystem.h"

#include "GLObjects/VertexArray.h"
#include "GLObjects/ArrayBuffer.h"
#include "GLObjects/IndexBuffer.h"
#include "GLObjects/UniformBuffer.h"
#include "GLObjects/Shader.h"

namespace vox
{
	struct Quad
	{
		Quad(const glm::vec3& c1, const glm::vec3& c2, const glm::vec3& c3, const glm::vec3& c4, const glm::u8vec4& color, const glm::vec3 normal) :
			c1(c1), c2(c2), c3(c3), c4(c4), color(color), normal(normal)
		{}

		const glm::vec3& c1;
		const glm::vec3& c2;
		const glm::vec3& c3;
		const glm::vec3& c4;
		const glm::u8vec4& color;
		const glm::vec3& normal;
	};

	//////////////////////////////////////////////////////////////////////////
	// Renderer class ////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class Renderer :
		public Subsystem
	{
	public:
		Renderer();
		~Renderer();



		void batchDrawQuad(Quad quad);

		void flushBatch();

		void onUpdate() override;

	private:
		float m_Fov;
		float m_MaxDistance;

		std::unique_ptr<Shader> m_DefaultShader;	// These stupid things have to be pointers so that destructor isn't run when using copy constructor -_-

		std::unique_ptr<VertexArray> m_QuadVao;
		/**
		 * Vertices stored as:
		 * float[3] vertices
		 * uint32	color (packed)
		 * uint32	normal (packed)
		*/
		std::unique_ptr<uint32_t[]> m_QuadBase;
		uint32_t* m_QuadPtr;
		std::unique_ptr<ArrayBuffer> m_QuadArrayBuffer;

		std::unique_ptr<GLuint[]> m_QuadIndices;
		unsigned m_QuadCount;
		std::unique_ptr<IndexBuffer> m_QuadIndexBuffer;
	};
}



