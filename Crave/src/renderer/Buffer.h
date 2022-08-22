#pragma once

namespace Crave
{
    class VertexLayout
    {
    public:
        struct VertexAttribute
        {
            unsigned type;
            unsigned count;
            unsigned normalized;

            const unsigned GetTypeSize() const;
            VertexAttribute(unsigned t, unsigned c, unsigned n)
                : type(t), count(c), normalized(n) {}
            VertexAttribute(unsigned t, unsigned c)
                : type(t), count(c) {}
            VertexAttribute() {}
        };

    public:
        VertexLayout(const std::initializer_list<VertexAttribute>& list);

        VertexLayout()
            : m_Stride(0) {}
        ~VertexLayout() {}

        const auto& Attribs() const { return m_Attribs; }
        unsigned    Stride()  const { return m_Stride; }

        void Enable() const;

    private:
        std::vector<VertexAttribute> m_Attribs;
        unsigned m_Stride;
    };

    class VBO
    {
    public:
        VBO() {}
        VBO(const void* data, const std::size_t size);
        VBO(const void* data, const std::size_t size, const int vertexCount);

        ~VBO();

        int Count() const { return m_Count; }
        int Id()    const { return m_Id; }

        void Bind() const;
        void Unbind() const;
        void SetLayout(const VertexLayout& layout) { m_Layout = layout; };
        const VertexLayout& GetLayout() const { return m_Layout; };

    private:
        unsigned m_Id;
        int m_Count;
        VertexLayout m_Layout;
    };

    class EBO
    {

    public:
        EBO() : m_Id(0), m_Count(0) {}
        EBO(const unsigned int* data, unsigned int m_Count);
        ~EBO();

        void Bind();
        void Unbind();

        unsigned Id()    const { return m_Id; }
        unsigned Count() const { return m_Count; }

    private:
        unsigned int m_Id;
        unsigned int m_Count;
    };

    class ShaderBlock
    {
    public:
        ShaderBlock(const char* name, const void* data,
            const std::size_t size, const unsigned type);
        ~ShaderBlock();


        void Upload(const void* data, const std::size_t size,
            const unsigned offset);
        const char* Name() const { return m_Name.c_str(); }
        const unsigned Type() const { return m_TypeUInt; }

        void Bind(unsigned bindingPoint);
        void Unbind();

        const unsigned Id() const { return m_Id; }
    private:
        std::string m_Name;
        unsigned m_Id;
        unsigned m_TypeUInt;
    };
}