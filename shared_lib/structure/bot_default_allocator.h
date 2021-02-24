#ifndef INCLUDE_BOT_DEFAULT_ALLOCATOR
#define INCLUDE_BOT_DEFAULT_ALLOCATOR

#include <memory>
#include <utility>

namespace bot {

template <typename T>
class DefaultAllocator {
public:
    using PtrType = std::shared_ptr<DefaultAllocator>;

    static constexpr const PtrType &instance()
    {
        return k_instance;
    }

    template <typename... Args>
    T *alloc(Args&&... args)
    {
        return new T(std::forward<Args>(args)...);
    }

    void free(T *t)
    {
        delete t;
    }

private:
    static std::shared_ptr<DefaultAllocator> k_instance;
};

template <typename T>
typename DefaultAllocator<T>::PtrType
    DefaultAllocator<T>::k_instance(new DefaultAllocator);

} // end of namespace bot

#endif
