#pragma once

#include <glm/glm.hpp>

namespace Crave
{
	class Shader
	{
	public:
		enum class Type {
			NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
		};
	public:
		Shader(const std::string& shaderPath);
		Shader(const std::unordered_map<Type, std::string> config);
		
		void Bind() const;

		void setBool(const std::string& name, bool value);

		void setInt(const std::string& name, int value);
		void setInt2(const std::string& name, int v0, int v1);
		void setInt2(const std::string& name, const glm::vec2& vec);

		void setUint(const std::string& name, unsigned value);
		void setFloat(const std::string& name, float value);

		void setFloat2(const std::string& name, float v0, float v1);

		void setFloat3(const std::string& name, float v0, float v1, float v2);
		void setFloat3(const std::string& name, const glm::vec3& vec);

		void setFloat4(const std::string& name, float v0, float v1, float v2, float v3);
		void setFloat4(const std::string& name, const glm::vec4& vec);

		void setMat4f(const std::string& name, const glm::mat4& matrix);

		const unsigned Id() const { return m_ProgramId; }
	private:
		void Parse(const std::string& shaderPath);
		void Parse(const std::unordered_map<Type, std::string>& config);
		void ParseIncludes(std::string& code, std::string filename);
		void Compile();

		void Link();
		const int GetUniformLocation(const std::string& name);

		void checkCompileErrors(unsigned int shader, std::string type);
	private:
		struct ShaderTypeData
		{
			std::string code;
			unsigned id;
			//bool found = false;
		};

		std::unordered_map<Type, ShaderTypeData> m_Data;
		std::string m_FullPath;
		unsigned int m_ProgramId;
		std::unordered_map<std::string, int> uniformLocationCache;

		static constexpr const char* BASE_SHADER_PATH = "res/shaders/";
	};

}