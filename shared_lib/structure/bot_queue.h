#ifndef INCLUDE_BOT_QUEUE
#define INCLUDE_BOT_QUEUE

#include <vector>

namespace bot {

template <typename T>
class Queue {
public:
    Queue();

    virtual ~Queue();

    void init(int size);

    bool empty() const
    {
        return m_first == -1;
    }

    bool full() const
    {
        return (m_last + 1) % static_cast<int>(m_q.size()) == m_first;
    }

    void clear();

    void enqueue(const T& t);

    void dequeue(T& t);

private:
    std::vector<T> m_q;
    int m_first;
    int m_last;
};

template <typename T>
Queue<T>::Queue()
: m_first(-1)
, m_last(-1)
{
}

template <typename T>
Queue<T>::~Queue()
{
}

template <typename T>
void Queue<T>::init(int size)
{
    m_q.resize(size);
}

template <typename T>
void Queue<T>::clear()
{
    m_first = -1;
    m_last = -1;
}

template <typename T>
void Queue<T>::enqueue(const T& t)
{
    m_last = (m_last + 1) % static_cast<int>(m_q.size());
    m_q[m_last] = t;
    if (m_first == -1)
    {
        m_first = 0;
    }
}

template <typename T>
void Queue<T>::dequeue(T& t)
{
    t = m_q[m_first];
    if (m_first != m_last)
    {
        m_first = (m_first + 1) % static_cast<int>(m_q.size());
    }
    else
    {
        m_first = -1;
        m_last = -1;
    }
}

} // end of namespace bot

#endif

