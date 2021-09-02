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
		bool loadFile(std::string filePath);

		/**
		 * @brief Copies all vertices in loader's vertex list into float pointer (array) in interleaved form in the order: pos, col, norm
		 * @param oArrPtr pointer to front of float array
		*/
		void setBuffer(unsigned int* oArrPtr) const;
		int getVertexCount() const;
		int getDataSize() const;
	private:
		std::vector<Vertex> mVertexList;
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

