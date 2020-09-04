#ifndef INCLUDE_BOT_LINKED_LIST
#define INCLUDE_BOT_LINKED_LIST

namespace bot {

template <typename T>
class LinkedList {
public:
    LinkedList()
        : m_first(nullptr)
    {}

    ~LinkedList()
    {}

    bool isEmpty() const
    {
        return m_first == nullptr;
    }

    T* getFirst()
    {
        return m_first;
    }

    const T* getFirst() const
    {
        return m_first;
    }

    void add(T* elem);

    bool unlink(T* elem);

    void unlink(T* prev, T* elem);

    template <typename MATCHER>
    T* matchAndUnlink(MATCHER& matcher);

    template <typename PROCESSOR>
    int forEach(PROCESSOR& processor);

    template <typename DEALLOCATOR>
    void clear(DEALLOCATOR& deallocator);

    void clear();

private:
    T* m_first;
};

template <typename T>
void LinkedList<T>::add(T* elem)
{
    elem->setNext(m_first);
    m_first = elem;
}

template <typename T>
bool LinkedList<T>::unlink(T* elem)
{
    T* prev = nullptr, * cur;

    for (cur = m_first; cur; cur = static_cast<T*>(cur->getNext()))
    {
        if (cur == elem)
        {
            break;
        }
        prev = cur;
    }

    if (!cur)
    {
        return false;
    }

    if (prev)
    {
        prev->setNext(cur->getNext());
    }
    else
    {
        m_first = cur->getNext();
    }
}

template <typename T>
void LinkedList<T>::unlink(T* prev, T* elem)
{
    if (prev)
    {
        prev->setNext(elem->getNext());
    }
    else
    {
        m_first = static_cast<T*>(elem->getNext());
    }
}

template <typename T>
template <typename MATCHER>
T* LinkedList<T>::matchAndUnlink(MATCHER& matcher)
{
    T* prev = nullptr, * cur;
    for (cur = m_first; cur; cur = static_cast<T*>(cur->getNext()))
    {
        if (matcher(cur))
        {
            break;
        }
        prev = cur;
    }

    if (!cur)
    {
        return nullptr;
    }

    if (prev)
    {
        prev->setNext(cur->getNext());
    }
    else
    {
        m_first = static_cast<T*>(cur->getNext());
    }

    return cur;
}

template <typename T>
template <typename PROCESSOR>
int LinkedList<T>::forEach(PROCESSOR& processor)
{
    for (T* cur = m_first; cur; cur = static_cast<T*>(cur->getNext()))
    {
        int rc = processor(*cur);

        if (0 != rc)
        {
            return rc;
        }
    }

    return 0;
}

template <typename T>
template <typename DEALLOCATOR>
void LinkedList<T>::clear(DEALLOCATOR& deallocator)
{
    T* next;
    for (T* cur = m_first; cur; cur = next)
    {
        next = static_cast<T*>(cur->getNext());
        deallocator(cur);
    }
    m_first = nullptr;
}

template <typename T>
void LinkedList<T>::clear()
{
    T* next;
    for (T* cur = m_first; cur; cur = next)
    {
        next = static_cast<T*>(cur->getNext());
        delete cur;
    }
    m_first = nullptr;
}

} // end of namespace bot

#endif
