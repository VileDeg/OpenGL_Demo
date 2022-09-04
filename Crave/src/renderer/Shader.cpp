#include "pch.h"
#include "Shader.h"

#include "glad/glad.h"

namespace Crave
{
	Shader::Shader(const char* shaderPath)
	{
		Parse(shaderPath);
		Compile();
		Link();
	}

	void Shader::Parse(const char* shaderPath)
	{
		std::ifstream shaderFile;

		try
		{
			m_FullPath = BASE_SHADER_PATH;
			m_FullPath += shaderPath;

			shaderFile.open(m_FullPath);

			ShaderType type = ShaderType::NONE;
			std::string line;
			std::stringstream ss[3];

			while (getline(shaderFile, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
					{
						type = ShaderType::VERTEX;
					}
					else if (line.find("fragment") != std::string::npos)
					{
						type = ShaderType::FRAGMENT;
					}
					else if (line.find("geometry") != std::string::npos)
					{
						type = ShaderType::GEOMETRY;
					}
					else
					{
						std::cout << "ERROR::SHADER::TYPE_NOT_SPECIFIED: " << line << std::endl;
						shaderFile.close();
						__debugbreak();
						return;
					}
					m_Data[type].found = true;
				}
				else
				{
					if (type == ShaderType::NONE)
					{
						std::cout << "ERROR::SHADER::INVALID_SHADER_TYPE: " << line << std::endl;
						shaderFile.close();
						__debugbreak();
						return;
					}
					ss[(int)type] << line << '\n';
				}
			}

			shaderFile.close();

			m_Data[ShaderType::VERTEX].code = ss[(int)ShaderType::VERTEX].str();
			m_Data[ShaderType::FRAGMENT].code = ss[(int)ShaderType::FRAGMENT].str();
			m_Data[ShaderType::GEOMETRY].code = ss[(int)ShaderType::GEOMETRY].str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
			__debugbreak();
		}
	}

	void Shader::Compile()
	{
		auto& v = m_Data[ShaderType::VERTEX];
		const char* shaderCode = v.code.c_str();
		// vertex shader
		v.id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(v.id, 1, &shaderCode, NULL);
		glCompileShader(v.id);
		checkCompileErrors(v.id, "VERTEX");
		// fragment Shader
		auto& f = m_Data[ShaderType::FRAGMENT];
		shaderCode = f.code.c_str();
		f.id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(f.id, 1, &shaderCode, NULL);
		glCompileShader(f.id);
		checkCompileErrors(f.id, "FRAGMENT");
		// geometry Shader

		auto& g = m_Data[ShaderType::GEOMETRY];
		if (!g.found)
			return;
		shaderCode = g.code.c_str();
		g.id = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(g.id, 1, &shaderCode, NULL);
		glCompileShader(g.id);
		checkCompileErrors(g.id, "GEOMETRY");
	}

	void Shader::Link()
	{
		m_ProgramId = glCreateProgram();
		for (auto& pair : m_Data)
		{
			if (!pair.second.found)
				continue;
			glAttachShader(m_ProgramId, pair.second.id);
		}

		glLinkProgram(m_ProgramId);
		checkCompileErrors(m_ProgramId, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		for (auto& pair : m_Data)
		{
			if (!pair.second.found)
				continue;
			glDeleteShader(pair.second.id);
		}
	}

	const int Shader::GetUniformLocation(const std::string& name)
	{
		if (uniformLocationCache.find(name) != uniformLocationCache.end())
			return uniformLocationCache[name];
		int location = glGetUniformLocation(m_ProgramId, name.c_str());
		if (location == -1)
			std::cout << "Warning: uniform " << name << " doesn't exist!\n";
		return uniformLocationCache[name] = location;
		return location;
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ProgramId);
	}
	
	void Shader::setBool(const std::string& name, bool value)
	{
		glUniform1i(GetUniformLocation(name), (int)value);
	}
	
	void Shader::setInt(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}
	void Shader::setUint(const std::string& name, unsigned value)
	{
		glUniform1ui(GetUniformLocation(name), value);
	}
	
	void Shader::setFloat(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}
	void Shader::setFloat2(const std::string& name, float v0, float v1)
	{
		glUniform2f(GetUniformLocation(name), v0, v1);
	}
	void Shader::setFloat3(const std::string& name, float v0, float v1, float v2)
	{
		glUniform3f(GetUniformLocation(name), v0, v1, v2);
	}
	void Shader::setFloat3(const std::string& name, const glm::vec3& vec)
	{
		glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
	}
	void Shader::setFloat4(const std::string& name, float v0, float v1, float v2, float v3)
	{
		glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
	}
	void Shader::setFloat4(const std::string& name, const glm::vec4& vec)
	{
		glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
	}
	void Shader::setMat4f(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	void Shader::checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type
					<< ".\nPATH: " << m_FullPath
					<< "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				__debugbreak();
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
					<< ".\nPATH: " << m_FullPath
					<< "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				__debugbreak();
			}
		}
	}
}