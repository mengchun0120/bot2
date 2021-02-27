#ifndef INCLUDE_DOUBLE_LINKED_LIST
#define INCLUDE_DOUBLE_LINKED_LIST

namespace bot {

template <typename T>
class DoubleLinkedList {
public:
    DoubleLinkedList()
        : m_first(nullptr)
    {}

    ~DoubleLinkedList()
    {}

    T *getFirst()
    {
        return m_first;
    }

    const T *getFirst() const
    {
        return m_first;
    }

    void add(T *t);

    void unlink(T *t);

    template <typename MATCHER>
    T *find(MATCHER &matcher);

    template <typename PROCESSOR>
    int forEach(PROCESSOR &processor);

    template <typename DEALLOCATOR>
    void clear(DEALLOCATOR &deallocator);

    void clear();

private:
    T *m_first;
};

template <typename T>
void DoubleLinkedList<T>::add(T *t)
{
    if (m_first)
    {
        m_first->setPrev(t);
    }
    t->setPrev(nullptr);
    t->setNext(m_first);
    m_first = t;
}

template <typename T>
void DoubleLinkedList<T>::unlink(T *t)
{
    T *prev = static_cast<T*>(t->getPrev());
    T *next = static_cast<T*>(t->getNext());

    if (prev)
    {
        prev->setNext(next);
    }
    else
    {
        m_first = next;
    }

    if (next)
    {
        next->setPrev(prev);
    }
}

template <typename T>
template <typename MATCHER>
T *DoubleLinkedList<T>::find(MATCHER &matcher)
{
    for (T *t = m_first; t; t = static_cast<T*>(t->getNext()))
    {
        if (matcher(t))
        {
            return t;
        }
    }

    return nullptr;
}

template <typename T>
template <typename PROCESSOR>
int DoubleLinkedList<T>::forEach(PROCESSOR &processor)
{
    for (T *t = m_first; t; t = static_cast<T*>(t->getNext()))
    {
        int rc = processor(t);
        if (0 != rc)
        {
            return rc;
        }
    }

    return 0;
}

template <typename T>
template <typename DEALLOCATOR>
void DoubleLinkedList<T>::clear(DEALLOCATOR &deallocator)
{
    T *next;
    for (T *t = m_first; t; t = next)
    {
        next = static_cast<T*>(t->getNext());
        deallocator(t);
    }
    m_first = nullptr;
}

template <typename T>
void DoubleLinkedList<T>::clear()
{
    T *next;
    for (T *t = m_first; t; t = next)
    {
        next = static_cast<T*>(t->getNext());
        delete t;
    }
    m_first = nullptr;
}

} // end of namespace bot

#endif
