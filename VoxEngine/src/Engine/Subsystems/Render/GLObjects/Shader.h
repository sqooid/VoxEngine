#pragma once

class Shader
{
public:
	Shader() = default;
	Shader(const std::string& filePath);
	~Shader();

	void loadShader(const std::string& filePath);
	void use() const;

	void bindUniformBlock(std::string uniformName, int bufferIndex);
private:
	GLuint mProgram;
};

