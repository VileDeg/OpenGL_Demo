#pragma once

#include "math_headers.h"

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertNFragPath);


	void Parse(const char* vertexPath, const char* fragmentPath);
	void Parse(const char* vertNFragPath);
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

	const unsigned Id() const { return ID; }
private:
	static constexpr const char* BASE_SHADER_PATH = "res/shaders/";

	std::string m_VertexCode;
	std::string m_FragmentCode;
    std::string m_VertFullPath;
    std::string m_FragFullPath;
	unsigned int ID;
	unsigned int vShader;
	unsigned int fShader;
	std::unordered_map<std::string, int> uniformLocationCache;

	const int GetUniformLocation(const std::string& name);

	void checkCompileErrors(unsigned int shader, std::string type);
};

