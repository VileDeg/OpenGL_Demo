#pragma once

#include "glad/glad.h"
#include <cstddef>

class VBO
{
public:
    VBO(const void* data, const std::size_t size);
    VBO(const void* data, const std::size_t size, const int vertexCount);
    VBO() {}

    ~VBO();

    inline const int Count() const { return count; }

    void Bind() const;
    void Unbind() const;
private:
    unsigned id;
    int count;
};


