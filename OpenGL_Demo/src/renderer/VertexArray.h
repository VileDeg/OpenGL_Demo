#pragma once
#include "Buffer.h"

class VAO
{
public:
    VAO();
    ~VAO();
    void AddBuffer(const VBO& vbo, Ref<EBO> ebo);

    const int Count() const { return m_Count; }
    Ref<EBO> Ebo() { return m_EBO; }

    void Bind()   const;
    void Unbind() const;

    const unsigned Id() const { return m_Id; }
private:
    unsigned m_Id;
    int      m_Count;
    Ref<EBO> m_EBO;
private:
    friend class cereal::access;
    template<typename Archive>
    void serialize(Archive& ar)
    {
        ar& cereal::make_nvp("ID", m_Id);
        ar& cereal::make_nvp("Count", m_Count);
        ar& cereal::make_nvp("IndexBuffer", m_EBO);
    }
};