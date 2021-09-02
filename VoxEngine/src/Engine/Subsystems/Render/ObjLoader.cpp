#include "voxpch.h"
#include "ObjLoader.h"

#include <cstdlib>
#include <cstdio>

#include <glm/gtc/packing.hpp>

namespace vox
{
	// Forward declarations
	glm::vec3 splitSpaceVec3(std::istringstream& s_line);
	std::string makeRelativePath(std::string baseFilePath, std::string auxFile);
	glm::vec2 splitSpaceVec2(std::istringstream& s_line);


	/**
	 * @brief Loads and parses .obj file and stores vertex data in current ObjLoader instance
	 * @param filePath Path to .obj to be loaded
	 * @return true if successfully loaded object
	*/
	bool ObjLoader::loadFile(std::string filePath)
	{
		std::ifstream file(filePath);

		// Check if file successfully opened
		if (!file.is_open())
		{
			return false;
		}

		// Buffers for values
		std::vector<glm::vec3> vertices;
		std::vector<unsigned int> normals;
		std::vector<unsigned int> colors;

		// PNG loader
		PngLoader pngLoader;

		// Reading values
		for (std::string line; std::getline(file, line); )
		{
			std::istringstream s_line(line);
			std::string head;
			std::getline(s_line, head, ' ');

			// Material
			if (head == "mtllib")
			{
				std::string mtlFilename;
				std::getline(s_line, mtlFilename);
				std::ifstream mtlFile(makeRelativePath(filePath, mtlFilename));

				for (std::string mtlLine; std::getline(mtlFile, mtlLine); )
				{
					std::istringstream s_mtlLine(mtlLine);
					std::string mtlToken;
					std::getline(s_mtlLine, mtlToken, ' ');
					if (mtlToken == "map_Kd")
					{
						std::getline(s_mtlLine, mtlToken);
						pngLoader.loadFile(makeRelativePath(filePath, mtlToken));
					}
				}
			}

			//

			// Vertex
			else if (head == "v")
			{
				vertices.emplace_back(splitSpaceVec3(s_line));
			}

			// Normal
			else if (head == "vn")
			{
				glm::vec4 temp = glm::vec4(splitSpaceVec3(s_line), 0.0f);
				normals.emplace_back(glm::packSnorm4x8(temp));
			}

			// colors
			else if (head == "vt")
			{
				glm::vec2 pixelCoord = splitSpaceVec2(s_line);
				colors.emplace_back(glm::packUint4x8(pngLoader.getPixel(static_cast<int>(pixelCoord.x * pngLoader.getWidth()), static_cast<int>(pixelCoord.y * pngLoader.getHeight()))));
			}

			else if (head == "f")
			{
				for (std::string vertex; std::getline(s_line, vertex, ' '); )
				{
					std::istringstream s_vertex(vertex);
					std::string index;
					// Vertex index
					std::getline(s_vertex, index, '/');
					glm::vec3& pos = vertices[std::stoi(index) - 1];

					// Color index
					std::getline(s_vertex, index, '/');
					unsigned int& col = colors[std::stoi(index) - 1];

					// Normal index
					std::getline(s_vertex, index);
					unsigned int& norm = normals[std::stoi(index) - 1];

					mVertexList.emplace_back(Vertex(pos, col, norm));
				}
			}
		}

		return true;
	}

	void ObjLoader::setBuffer(uint32_t* oArrPtr) const
	{
		int i = 0;
		for (const Vertex& vert : mVertexList)
		{
			oArrPtr[i++] = *reinterpret_cast<const uint32_t*>(&vert.mPosition.x);
			oArrPtr[i++] = *reinterpret_cast<const uint32_t*>(&vert.mPosition.y);
			oArrPtr[i++] = *reinterpret_cast<const uint32_t*>(&vert.mPosition.z);

			oArrPtr[i++] = vert.mColor;

			oArrPtr[i++] = vert.mNormal;
		}
	}

	int ObjLoader::getVertexCount() const
	{
		return mVertexList.size();
	}

	int ObjLoader::getDataSize() const
	{
		return mVertexList.size() * 5 * 4;
	}

	glm::vec3 splitSpaceVec3(std::istringstream& s_line)
	{
		glm::vec3 vector;
		std::string value;
		std::getline(s_line, value, ' ');
		vector.x = std::stof(value);
		std::getline(s_line, value, ' ');
		vector.y = std::stof(value);
		std::getline(s_line, value);
		vector.z = std::stof(value);
		return vector;
	}

	glm::vec2 splitSpaceVec2(std::istringstream& s_line)
	{
		glm::vec3 vector;
		std::string value;
		std::getline(s_line, value, ' ');
		vector.x = std::stof(value);
		std::getline(s_line, value);
		vector.y = std::stof(value);

		return vector;
	}

	std::string makeRelativePath(std::string baseFilePath, std::string auxFile)
	{
		std::filesystem::path parentPath(baseFilePath);
		parentPath = parentPath.parent_path();
		std::filesystem::path auxPath(auxFile);
		auxPath = parentPath / auxPath;
		return auxPath.string();
	}

	PngLoader::~PngLoader()
	{
		for (int y = 0; y < mHeight; ++y)
		{
			free(mRowPtrs[y]);
		}
		free(mRowPtrs);
	}

	bool PngLoader::loadFile(std::string filePath)
	{
		FILE* fp = fopen(filePath.c_str(), "rb");

		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png) abort();

		png_infop info = png_create_info_struct(png);
		if (!info) abort();

		if (setjmp(png_jmpbuf(png))) abort();

		png_init_io(png, fp);
		png_read_info(png, info);

		mWidth = png_get_image_width(png, info);
		mHeight = png_get_image_height(png, info);
		mColorType = png_get_color_type(png, info);
		mBitDepth = png_get_bit_depth(png, info);

		//png_read_update_info(png, info);

		mRowPtrs = (png_bytep*)malloc(sizeof(png_bytep) * mHeight);
		if (!mRowPtrs) abort();
		for (int y = 0; y < mHeight; ++y)
		{
			mRowPtrs[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
		}

		png_read_image(png, mRowPtrs);
		fclose(fp);

		png_destroy_read_struct(&png, &info, NULL);

		return true;
	}

	glm::u8vec4 PngLoader::getPixel(int x, int y) const
	{
		png_bytep row = mRowPtrs[y];
		png_bytep px = &(row[x * 4]);

		return glm::u8vec4(px[0], px[1], px[2], px[3]);
	}

	int PngLoader::getWidth() const
	{
		return mWidth;
	}

	int PngLoader::getHeight() const
	{
		return mHeight;
	}

	Vertex::Vertex(glm::vec3 position, unsigned int color, unsigned int normal) : mPosition(position), mColor(color), mNormal(normal)
	{
	}
}
