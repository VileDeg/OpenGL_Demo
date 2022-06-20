#pragma once

#include "glad/glad.h"

class EBO
{
private:
    unsigned int id;
    unsigned int count;
    
public:
    EBO() : id(0), count(0) {}
    EBO(const unsigned int* data, unsigned int count);
    ~EBO();

    void Bind();
    void Unbind();

    const unsigned int Count() const { return count; }
};

