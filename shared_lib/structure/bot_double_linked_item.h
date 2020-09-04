#ifndef INCLUDE_BOT_DOUBLE_LINKED_ITEM
#define INCLUDE_BOT_DOUBLE_LINKED_ITEM

namespace bot {

class DoubleLinkedItem {
public:
    DoubleLinkedItem()
        : m_prev(nullptr)
        , m_next(nullptr)
    {}

    virtual ~DoubleLinkedItem()
    {}

    const DoubleLinkedItem* getPrev() const
    {
        return m_prev;
    }

    DoubleLinkedItem* getPrev()
    {
        return m_prev;
    }

    void setPrev(DoubleLinkedItem* prev)
    {
        m_prev = prev;
    }

    const DoubleLinkedItem* getNext() const
    {
        return m_next;
    }

    DoubleLinkedItem* getNext()
    {
        return m_next;
    }

    void setNext(DoubleLinkedItem* next)
    {
        m_next = next;
    }

protected:
    DoubleLinkedItem* m_prev, *m_next;
};

} // end of namespace bot

#endif
