#ifndef INCLUDE_BOT_OBJECT_POOL
#define INCLUDE_BOT_OBJECT_POOL

#include <vector>
#include <type_traits>

namespace bot {

template <typename T>
class ObjectPool {

public:
    ObjectPool()
        : m_free(nullptr)
        , m_pool(nullptr)
        , m_capacity(0)
    {}

    ObjectPool(int capacity)
    {
        init(capacity);
    }

    virtual ~ObjectPool()
    {
        delete[] m_pool;
    }

    void init(int capacity);

    T* alloc();

    void free(T* t);

protected:
    T* m_free;
    T* m_pool;
    int m_capacity;
};

template <typename T>
void ObjectPool<T>::init(int capacity)
{
    m_capacity = capacity;
    m_pool = new T[capacity];
    for (int i = 1; i < capacity; ++i)
    {
        m_pool[i - 1].setNext(&m_pool[i]);
    }
    m_free = &m_pool[0];
}

template <typename T>
T* ObjectPool<T>::alloc()
{
    T* ret = nullptr;
    if (m_free)
    {
        ret = m_free;
        m_free = static_cast<T*>(m_free->getNext());
    }
    else
    {
        ret = new T();
    }
    return ret;
}

template <typename T>
void ObjectPool<T>::free(T* t)
{
    if (t >= m_pool && t < m_pool + m_capacity)
    {
        t->setNext(m_free);
        m_free = t;
    }
    else
    {
        delete t;
    }
}

} // end of namespace bot

#endif
