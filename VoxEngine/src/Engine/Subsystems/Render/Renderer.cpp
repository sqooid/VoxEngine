#include "voxpch.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/packing.hpp>

#define BATCH_MAX_QUAD_COUNT 1024
#define BATCH_MAX_TRIANGLE_COUNT 1024

namespace vox
{
	Renderer::Renderer() :
		m_TriangleCount(0),
		m_QuadCount(0)
	{
		// OpenGL settings
		glEnable(GL_DEPTH_TEST);

		// Triangle batcher initialization
		m_TriangleVao = std::make_unique<VertexArray>();
		m_TriangleVao->pushAttribute<float>(3, false);
		m_TriangleVao->pushAttribute<unsigned char>(4, true);
		m_TriangleVao->pushAttribute<unsigned char>(4, true);
		const int vertexSizeTriangle = 5 * 4 * 3 * BATCH_MAX_TRIANGLE_COUNT;
		m_TriangleBase = std::make_unique<uint32_t[]>(vertexSizeTriangle);
		m_TrianglePtr = m_TriangleBase.get();
		m_TriangleArrayBuffer = std::make_unique<ArrayBuffer>(vertexSizeTriangle, m_TrianglePtr, BufferUsage::Dynamic);

		m_TriangleVao->bindBuffer(*m_TriangleArrayBuffer);

		m_QuadVao = std::make_unique<VertexArray>();
		m_QuadVao->pushAttribute<float>(3, false);
		m_QuadVao->pushAttribute<unsigned char>(4, true);
		m_QuadVao->pushAttribute<unsigned char>(4, true);

		// Quad batcher initialization
		const int vertexSizeQuad = 5 * 4 * 4 * BATCH_MAX_QUAD_COUNT;
		m_QuadBase = std::make_unique<uint32_t[]>(vertexSizeQuad);
		m_QuadPtr = m_QuadBase.get();
		m_QuadArrayBuffer = std::make_unique<ArrayBuffer>(vertexSizeQuad, m_QuadPtr, BufferUsage::Dynamic);

		m_QuadVao->bindBuffer(*m_QuadArrayBuffer);

		const int indexCount = 6 * BATCH_MAX_QUAD_COUNT;
		m_QuadIndices = std::make_unique<GLuint[]>(indexCount);
		int index = 0;
		for (int i = 0; index < indexCount; i += 4)
		{
			m_QuadIndices[index++] = i;
			m_QuadIndices[index++] = i + 1;
			m_QuadIndices[index++] = i + 2;

			m_QuadIndices[index++] = i + 2;
			m_QuadIndices[index++] = i + 3;
			m_QuadIndices[index++] = i;
		}
		m_QuadIndexBuffer = std::make_unique<IndexBuffer>(indexCount, m_QuadIndices.get());

		m_QuadVao->bindIndexBuffer(*m_QuadIndexBuffer);

		m_DefaultShader = std::make_unique<Shader>("shaders/one.glsl");
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::batchTriangle(Triangle triangle)
	{
		if (m_TriangleCount == BATCH_MAX_TRIANGLE_COUNT)
		{
			flushTriangle();
		}

		uint32_t normalPacked = glm::packSnorm4x8(glm::vec4(triangle.normal, 0.0f));
		uint32_t colorPacked = glm::packUint4x8(triangle.color);

		*m_TrianglePtr++ = *reinterpret_cast<const uint32_t*>(&triangle.c1.x);
		*m_TrianglePtr++ = *reinterpret_cast<const uint32_t*>(&triangle.c1.y);
		*m_TrianglePtr++ = *reinterpret_cast<const uint32_t*>(&triangle.c1.z);
		*m_TrianglePtr++ = colorPacked;
		*m_TrianglePtr++ = normalPacked;

		*m_TrianglePtr++ = *reinterpret_cast<const uint32_t*>(&triangle.c2.x);
		*m_TrianglePtr++ = *reinterpret_cast<const uint32_t*>(&triangle.c2.y);
		*m_TrianglePtr++ = *reinterpret_cast<const uint32_t*>(&triangle.c2.z);
		*m_TrianglePtr++ = colorPacked;
		*m_TrianglePtr++ = normalPacked;

		*m_TrianglePtr++ = *reinterpret_cast<const uint32_t*>(&triangle.c3.x);
		*m_TrianglePtr++ = *reinterpret_cast<const uint32_t*>(&triangle.c3.y);
		*m_TrianglePtr++ = *reinterpret_cast<const uint32_t*>(&triangle.c3.z);
		*m_TrianglePtr++ = colorPacked;
		*m_TrianglePtr++ = normalPacked;

		++m_TriangleCount;
	}

	void Renderer::batchTriangle(uint32_t* data)
	{
		if (m_TriangleCount == BATCH_MAX_TRIANGLE_COUNT)
		{
			flushTriangle();
		}

		auto col = *(data + 9);
		auto norm = *(data + 10);
		for (int i = 0; i < 3; ++i)
		{
			*m_TrianglePtr++ = *data++;
			*m_TrianglePtr++ = *data++;
			*m_TrianglePtr++ = *data++;
			*m_TrianglePtr++ = col;
			*m_TrianglePtr++ = norm;
		}

		++m_TriangleCount;
	}

	void Renderer::flushTriangle()
	{
		m_DefaultShader->use();
		m_TriangleArrayBuffer->setData(m_TriangleBase.get(), m_TriangleCount * 4 * 5 * 3);
		m_TriangleVao->bind();
		glDrawArrays(GL_TRIANGLES, 0, m_TriangleCount * 3);
		m_TriangleVao->unbind();
		m_TrianglePtr = m_TriangleBase.get();
		m_TriangleCount = 0;
	}

	void Renderer::batchQuad(Quad quad)
	{
		if (m_QuadCount == BATCH_MAX_QUAD_COUNT)
		{
			flushQuad();
		}

		uint32_t normalPacked = glm::packSnorm4x8(glm::vec4(quad.normal, 0.0f));
		uint32_t colorPacked = glm::packUint4x8(quad.color);

		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c1.x);
		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c1.y);
		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c1.z);
		*m_QuadPtr++ = colorPacked;
		*m_QuadPtr++ = normalPacked;

		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c2.x);
		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c2.y);
		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c2.z);
		*m_QuadPtr++ = colorPacked;
		*m_QuadPtr++ = normalPacked;

		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c3.x);
		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c3.y);
		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c3.z);
		*m_QuadPtr++ = colorPacked;
		*m_QuadPtr++ = normalPacked;

		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c4.x);
		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c4.y);
		*m_QuadPtr++ = *reinterpret_cast<const uint32_t*>(&quad.c4.z);
		*m_QuadPtr++ = colorPacked;
		*m_QuadPtr++ = normalPacked;

		++m_QuadCount;
	}

	void Renderer::flushQuad()
	{
		m_DefaultShader->use();
		m_QuadArrayBuffer->setData(m_QuadBase.get(), m_QuadCount * 4 * 5 * 4);
		m_QuadVao->bind();
		glDrawElements(GL_TRIANGLES, m_QuadCount * 6, GL_UNSIGNED_INT, 0);
		m_QuadVao->unbind();
		m_QuadPtr = m_QuadBase.get();
		m_QuadCount = 0;
	}

	void Renderer::onUpdate()
	{
		flushTriangle();
		flushQuad();
	}
}




