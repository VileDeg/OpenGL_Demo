#pragma once

#include "math_headers.h"

class Shader
{
public:
	Shader(const char* shaderPath);

	void Parse(const char* shaderPath);
	void Compile();
	
	void Link();
	void Bind() const;
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);


	void setFloat(const std::string& name, float value);

	void setFloat2(const std::string& name, float v0, float v1);

	void setFloat3(const std::string& name, float v0, float v1, float v2);
	void setFloat3(const std::string& name, const glm::vec3& vec);

	void setFloat4(const std::string& name, float v0, float v1, float v2, float v3);
	void setFloat4(const std::string& name, const glm::vec4& vec);

	void setMat4f(const std::string& name, const glm::mat4& matrix);

	const unsigned Id() const { return m_ProgramId; }
private:
	const int GetUniformLocation(const std::string& name);

	void checkCompileErrors(unsigned int shader, std::string type);
private:

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
	};

	struct ShaderTypeData
	{
		std::string code;
		unsigned id;
		bool found = false;
	};

	std::unordered_map<ShaderType, ShaderTypeData> m_Data;
	std::string m_FullPath;
	unsigned int m_ProgramId;
	std::unordered_map<std::string, int> uniformLocationCache;

	static constexpr const char* BASE_SHADER_PATH = "res/shaders/";
};

