#pragma once

#include "glad/glad.h"
#include <vector>
#include <iostream>

struct VertexAttribute
{
    unsigned type;
    unsigned count;
    unsigned normalized;

    const unsigned GetTypeSize() const
    {
        switch (type)
        {
        case GL_FLOAT:         return sizeof(GLfloat);
        case GL_UNSIGNED_INT:  return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
        }

        
        return 0;
    }
};

class VertexLayout
{

private:
    std::vector<VertexAttribute> attribs;
    unsigned stride;
public:
    VertexLayout()
        : stride(0) {}
    ~VertexLayout() {}

    template<typename T>
    void Push(unsigned count)
    {
        std::cerr << "Error: Push<T> called\n";
        abort();
    }
    template<>
    void Push<float>(unsigned count)
    {
        attribs.push_back({ GL_FLOAT, count, GL_FALSE });
        stride += count * sizeof(GLfloat);
    }
    template<>
    void Push<unsigned>(unsigned count)
    {
        attribs.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        stride += count * sizeof(GLuint);
    }
    template<>
    void Push<unsigned char>(unsigned count)
    {
        attribs.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        stride += count * sizeof(GLbyte);
    }

    inline const auto& Attribs() const { return attribs; }
    inline unsigned Stride() const { return stride; }

    void Enable() const;
};


