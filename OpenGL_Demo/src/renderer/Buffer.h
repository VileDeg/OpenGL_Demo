#pragma once

/////////////////////////////////////////////////////////
//Layout/////////////////////////////////////////////////
/////////////////////////////////////////////////////////

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
        
    };

public:
    VertexLayout(const std::initializer_list<VertexAttribute>& list);
   
    VertexLayout()
        : stride(0) {}
    ~VertexLayout() {}

    inline const auto& Attribs() const { return attribs; }
    inline unsigned Stride() const { return stride; }

    void Enable() const;
private:
    std::vector<VertexAttribute> attribs;
    unsigned stride;
};

/////////////////////////////////////////////////////////
//VBO////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

class VBO
{
public:
    VBO(const void* data, const std::size_t size);
    VBO(const void* data, const std::size_t size, const int vertexCount);

    ~VBO();

    inline const int Count() const { return count; }

    void Bind() const;
    void Unbind() const;
    void SetLayout(const VertexLayout& layout) { this->layout = layout; };
    const VertexLayout& GetLayout() const { return layout; };
private:
    unsigned id;
    int count;
    VertexLayout layout;
};

/////////////////////////////////////////////////////////
//EBO////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////
//UBO////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

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
    //enum Type m_Type;
    unsigned m_TypeUInt;
};