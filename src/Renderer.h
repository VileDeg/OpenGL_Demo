#pragma once

#include "glad/glad.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shader/Shader.h"

class Renderer
{
public:
	void Draw(const VAO& vao, const EBO& ebo, const Shader& shader) const;
	void DrawNoIndex(const VAO& vao, const Shader& shader) const;
	void Clear() const;
private:

};