#include "EBO.h"

EBO::EBO(const unsigned int* data, unsigned int count) : count(count)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
}

EBO::~EBO()
{
    glDeleteBuffers(1, &id);
}


void EBO::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
