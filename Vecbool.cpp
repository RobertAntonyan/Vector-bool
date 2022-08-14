#include <iostream>
#include <initializer_list>

enum
{
    bit_count = 4 * sizeof(size_t)
};

template <typename T>
class Vector;

template <>
class Vector<bool>
{
public:
    class Reference
    {
    public:
        // Reference();
        ~Reference();
        Reference(size_t *, size_t);
        Reference(const Reference &);
        Reference(Reference &&);
        Reference &operator=(const Reference &);
        Reference &operator=(Reference &&);
        Reference &operator=(bool);
        operator bool() const;
        Reference operator[](size_t);
        Reference operator[](size_t) const;
        void flip();

    private:
        size_t *ptr;
        size_t index;
        bool val;
    };

public:
    Vector() = default;
    Vector(const Vector &);
    Vector(Vector &&);
    Vector(std::initializer_list<bool>);
    Vector(size_t);
    Vector(size_t, bool);
    ~Vector();
    Reference operator[](size_t);
    bool empty();
    void insert(size_t, bool);
    void erase(size_t);
    void push_back(bool val);
    void pop_back();

private:
    size_t m_cap;
    size_t m_size;
    size_t *m_buf;

public:
};

Vector<bool>::Reference::Reference(size_t *pt, size_t ind)
    : ptr{pt}, index{ind}, val{}
{
    val = ptr[index / bit_count] & (1 << (index % bit_count));
}

void Vector<bool>::Reference::flip()
{
    if (val)
    {
        ptr[index / bit_count] ^= (1 << (index % bit_count));
    }
}

Vector<bool>::Reference::
    Reference(const Reference &oth)
    : ptr{oth.ptr}, index{oth.index}, val{oth.val}
{
    ptr = nullptr;
    index = 0;
    val = 0;
}

Vector<bool>::Reference::Reference(Reference &&oth) : ptr{oth.ptr}, index{oth.index}, val{oth.val}
{
    ptr = nullptr;
    index = 0;
    val = 0;
}

Vector<bool>::Reference &Vector<bool>::Reference::operator=(const Reference &oth)
{
    if (val != oth.val)
    {
        this->flip();
    }
    return *this;
}

Vector<bool>::Reference &Vector<bool>::Reference::
operator=(Reference &&oth)
{
    if (val != oth.val)
    {
        this->flip();
    }
    return *this;
}

Vector<bool>::Reference &Vector<bool>::Reference::Reference::operator=(bool val)
{
    if (this->val != val)
    {
        this->flip();
    }
    return *this;
}

Vector<bool>::Reference::~Reference()
{
}

Vector<bool>::Reference::operator bool() const
{

    return val;
}

Vector<bool>::Reference Vector<bool>::Reference::operator[](size_t pos)
{
    return Reference(ptr, pos);
}

Vector<bool>::Reference Vector<bool>::Reference::operator[](size_t pos) const
{
    return Reference(ptr, pos);
}

Vector<bool>::Vector(const Vector<bool> &oth) : m_cap{oth.m_cap}, m_size{oth.m_size}
{
    m_buf = new size_t[m_cap / bit_count];
    for (int i = 0; i < (m_cap / bit_count); ++i)
    {
        m_buf[i] = oth.m_buf[i];
    }
}

Vector<bool>::Vector(Vector &&oth) : m_cap{oth.m_cap}, m_size{oth.m_size}
{
    m_buf = new size_t[m_cap / bit_count];
    for (int i = 0; i < (m_cap / bit_count); ++i)
    {
        m_buf[i] = oth.m_buf[i];
    }
}

Vector<bool>::Vector(std::initializer_list<bool> init)
{
    if (init.size())
    {
        while (m_cap < init.size())
        {
            m_cap = bit_count;
        }
        m_buf = new size_t[m_cap / bit_count];
        size_t bit = 1;
        int i = 0;
        for (auto it = init.begin(); it != init.end(); ++it, ++i)
        {
            if (*it)
            {
                m_buf[i / bit_count] |= (1 << (i % bit_count));
            }
            bit <<= 1;
        }
    }
}
Vector<bool>::Vector(size_t count) : m_cap{}, m_size{count}, m_buf{}
{
    if (count)
    {
        while (m_cap < count)
        {
            m_cap += bit_count;
        }
        m_buf = new size_t[m_cap / bit_count];
    }
}

Vector<bool>::Vector(size_t count, bool val) : m_cap{}, m_size{count}, m_buf{}
{
    if (count)
    {
        while (m_cap < count)
        {
            m_cap += bit_count;
        }
        m_buf = new size_t[m_cap / bit_count];

        if (val)
        {
            size_t bit = 1;
            for (int i = 0; i < m_size; ++i)
            {
                m_buf[i / bit_count] |= bit;
                bit <<= 1;
            }
        }
    }
}
bool Vector<bool>::empty()
{
    if (m_size)
    {
        return false;
    }
    return true;
}
void Vector<bool>::insert(size_t pos, bool val)
{
    if (this->empty())
    {
        m_cap = bit_count;
        m_buf = new size_t[1];
        m_buf[m_size++] = val;
        return;
    }
    if (m_size == m_cap)
    {
        m_cap += bit_count;
        size_t *ptr = new size_t[m_cap / bit_count];
        for (int i = 0; i < m_size / bit_count; ++i)
        {
            ptr[i] = m_buf[i];
        }
        delete[] m_buf;
        m_buf = ptr;
        ptr = nullptr;
    }
    for (int i = m_size; i > pos; --i)
    {
        (*this)[i] = (*this)[i - 1];
    }
    (*this)[pos] = val;
    ++m_size;
}

void Vector<bool>::erase(size_t pos)
{
    if (this->empty())
    {

        return;
    }

    for (int i = m_size; i > pos; --i)

    {
        (*this)[i - 1] = (*this)[i];
    }

    --m_size;
}

Vector<bool>::~Vector()
{
}

Vector<bool>::Reference Vector<bool>::operator[](size_t pos)
{
    return Reference(m_buf, pos);
}

void Vector<bool>::push_back(bool val)
{
    if (this->empty())
    {
        m_cap = bit_count;
        m_buf = new size_t[1];
        (*this)[m_size++] = val;
        return;
    }
    if (m_size <= m_cap)
    {
        m_cap += bit_count;
        size_t *ptr = new size_t[m_cap / bit_count];
        for (int i = 0; i < (m_size / bit_count); ++i)
        {
            ptr[i] = m_buf[i];
        }
        delete[] m_buf;
        m_buf = ptr;
        ptr = nullptr;
    }
        (*this)[m_size++] = val;
}

void Vector<bool>::pop_back()
{
    (*this)[m_size--] = false;
}

int main()
{
    Vector<bool> b;
    b.push_back(true);
    b.push_back(true);
    b.push_back(true);
    
    std::cout << b[2] << "\n";
    
       

    
  
    
}
