#include "VBO.h"


VBO::VBO(const void* data, std::size_t size) : count(-1)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VBO::VBO(const void* data, const std::size_t size, const int vertexCount)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    count = vertexCount;
}

VBO::~VBO()
{
    glDeleteBuffers(1, &id);
}


void VBO::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
