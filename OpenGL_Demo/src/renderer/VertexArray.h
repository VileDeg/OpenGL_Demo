#pragma once
#include "Buffer.h"

class VAO
{
public:
    VAO();
    ~VAO();
    void AddBuffer(const VBO& vbo, EBO* ebo = nullptr);

    const int Count() const { return m_Count; }
    Ref<EBO> Ebo() { return m_EBO; }

    void Bind()   const;
    void Unbind() const;

    const unsigned Id() const { return m_Id; }
private:
    unsigned m_Id;
    int      m_Count;
    Ref<EBO> m_EBO;
};