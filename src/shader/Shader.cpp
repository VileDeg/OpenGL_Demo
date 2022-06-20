#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::string Shader::shader_path = "res/shaders/";

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	Parse(vertexPath, fragmentPath);
	Compile();
	Link();
}


void Shader::Parse(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	using namespace std;

	try
	{
		// open files
		std::string path = shader_path;
		path += vertexPath;
        vertFullPath = path;
		vShaderFile.open(path);

		path = shader_path;
		path += fragmentPath;
        fragFullPath = path;
		fShaderFile.open(path);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
	}
}

void Shader::Compile()
{
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders

	// vertex shader
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vShaderCode, NULL);
	glCompileShader(vShader);
	checkCompileErrors(vShader, "VERTEX");
	// fragment Shader
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderCode, NULL);
	glCompileShader(fShader);
	checkCompileErrors(fShader, "FRAGMENT");
}

void Shader::Link()
{
	ID = glCreateProgram();
	glAttachShader(ID, vShader);
	glAttachShader(ID, fShader);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

const int Shader::GetUniformLocation(const std::string& name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache[name];
	int location = glGetUniformLocation(ID, name.c_str());
	if (location == -1)
		std::cout << "Warning: uniform " << name << " doesn't exist!\n";
	return uniformLocationCache[name] = location;
	return location;
}

void Shader::Use() const
{
	glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(GetUniformLocation(name), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}
// ------------------------------------------------------------------------
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
void Shader::setFloat4(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
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
            << ".\nPATH: " << vertFullPath << " " << fragFullPath
            << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type
            << ".\nPATH: " << vertFullPath << " " << fragFullPath
            << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}