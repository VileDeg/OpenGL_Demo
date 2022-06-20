#pragma once

#include "glad/glad.h"
#include "VBO.h"
#include "EBO.h"
#include "VertexLayout.h"

class VAO
{
private:
    unsigned id;

public:
    VAO();
    ~VAO();
    void AddBuffer(const VBO& vbo, const VertexLayout& layout);

	void Bind() const;
	void Unbind() const;

    const unsigned Id() const {return id;}
};

