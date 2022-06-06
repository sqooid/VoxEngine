#pragma once

#include <png.h>

namespace vox
{
	struct Vertex
	{
		Vertex(glm::vec3 position, unsigned int color, unsigned int normal);
		glm::vec3 mPosition;
		unsigned int mColor;
		unsigned int mNormal;
	};

	class ObjLoader
	{
	public:
		ObjLoader();
		bool loadFile(std::string filePath, int corners);
		std::shared_ptr<std::vector<uint32_t>> getData();

		int getVertexCount() const;
		int getDataSize() const;
	private:
		std::shared_ptr<std::vector<uint32_t>> m_VertexList;
	};

	class PngLoader
	{
	public:
		~PngLoader();

		bool loadFile(std::string filePath);
		glm::u8vec4 getPixel(int x, int y) const;
		int getWidth() const;
		int getHeight() const;

	private:
		int mWidth;
		int mHeight;
		png_byte mColorType;
		png_byte mBitDepth;
		png_bytep* mRowPtrs;
	};
}

