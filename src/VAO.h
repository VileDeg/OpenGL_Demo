#pragma once

#include "VBO.h"
#include "EBO.h"
#include "VertexLayout.h"

class VAO
{
public:
    VAO();
    ~VAO();
    void AddBuffer(const VBO& vbo, const VertexLayout& layout);

    inline const int Count() const { return count; }

	void Bind() const;
	void Unbind() const;

    const unsigned Id() const {return id;}
private:
    unsigned id;
    int count;
};

