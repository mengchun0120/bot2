#ifndef INCLUDE_BOT_LINKED_ITEM
#define INCLUDE_BOT_LINKED_ITEM

namespace bot {

class LinkedItem {
public:
    LinkedItem()
        : m_next(nullptr)
    {}

    virtual ~LinkedItem()
    {}

    const LinkedItem* getNext() const
    {
        return m_next;
    }

    LinkedItem* getNext()
    {
        return m_next;
    }

    void setNext(LinkedItem *next)
    {
        m_next = next;
    }

protected:
    LinkedItem *m_next;
};

} // end of namespace bot

#endif
