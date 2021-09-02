#include "voxpch.h"
#include "Shader.h"

enum class ShaderType
{
	VERTEX = 0,
	FRAGMENT = 1,
	GEOMETRY = 2
};

static bool compileShader(const GLuint& shaderID, const std::string& shaderSource, const std::string& filePath)
{
	const char* source = shaderSource.c_str();
	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);

	// Checking for errors
#ifdef DEBUG
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);
		std::cerr << "Error in shader compilation in file: " << filePath << ":\n" << &errorLog[0];

		glDeleteShader(shaderID); // Don't leak the shader.
		return false;
	}
#endif
	return true;
}

/**
 * @brief Loads shader file containing all shaders for a single program at once
 * Shader type should be preceded by #type <shader_type>
 * @param filePath Relative path of shader file to working directory
*/
void Shader::loadShader(const std::string& filePath)
{
	std::ifstream file(filePath);
	std::string shaderString;
	std::vector<GLuint>shaderObjs;

	for (std::string line; std::getline(file, line); )
	{
		std::istringstream lineStream(line);
		std::string token;
		std::getline(lineStream, token, ' ');

		// Detecting shader type switch
		if (token == "#type")
		{
			// Compile previous shader if there is one existing in text cache
			if (!shaderObjs.empty())
			{
				if (!compileShader(shaderObjs.back(), shaderString, filePath))
				{
					return; // Shader compilation failed, bail
				}
				// Clear previous string
				shaderString.clear();
			}

			std::getline(lineStream, token);
			if (token == "vertex")
			{
				shaderObjs.push_back(glCreateShader(GL_VERTEX_SHADER));
			}
			else if (token == "fragment")
			{
				shaderObjs.push_back(glCreateShader(GL_FRAGMENT_SHADER));
			}
			else if (token == "geometry")
			{
				shaderObjs.push_back(glCreateShader(GL_GEOMETRY_SHADER));
			}
			else
			{
				std::cerr << "Unrecognized shader type while loading shader: " << filePath << "\n";
				return;
			}

			continue;
		}
		if (!line.empty())
		{
			shaderString.append(line);
			shaderString.push_back('\n');
		}
	}
	// Compile last shader
	if (!shaderObjs.empty())
	{
		if (!compileShader(shaderObjs.back(), shaderString, filePath))
		{
			return; // Shader compilation failed, bail
		}
	}

	// Create program
	mProgram = glCreateProgram();

	// Attach shaders
	for (auto shaderID : shaderObjs)
	{
		glAttachShader(mProgram, shaderID);
	}

	// Linking shader
	glLinkProgram(mProgram);

	// Error checking
#ifdef DEBUG
	GLint isLinked = 0;
	glGetProgramiv(mProgram, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(mProgram, maxLength, &maxLength, &infoLog[0]);

		std::cerr << "Error in shader program from shader file: " << filePath << ":\n" << &infoLog[0];

		glDeleteProgram(mProgram);

		return;
	}
#endif
	// Cleanup
	for (auto shaderID : shaderObjs)
	{
		glDetachShader(mProgram, shaderID);
		glDeleteShader(shaderID);
	}
}

Shader::Shader(const std::string& filePath)
	: mProgram(0)
{
	loadShader(filePath);
}

Shader::~Shader()
{
}

void Shader::use() const
{
	glUseProgram(mProgram);
}

void Shader::bindUniformBlock(std::string uniformName, int bufferIndex)
{
	use();
	int uniformIndex = glGetUniformBlockIndex(mProgram, uniformName.c_str());
	glUniformBlockBinding(mProgram, uniformIndex, bufferIndex);
}
