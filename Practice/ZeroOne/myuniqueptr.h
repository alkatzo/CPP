#pragma once
#include <utility>

template<typename Tp>
struct default_deleter {
    constexpr default_deleter() noexcept = default;

    template<typename _Up,
             typename = std::_Require<std::is_convertible<_Up*, Tp*>>>
    default_deleter(const default_deleter<_Up>&) noexcept { }

    void operator()(Tp* p) {
        static_assert(!std::is_void<Tp>::value,
                      "can't delete pointer to incomplete type");
        static_assert(sizeof(Tp)>0,
                      "can't delete pointer to incomplete type");
        delete p;
    }
};

template<typename Tp>
struct default_deleter<Tp[]> {
    void operator()(Tp* p) {
        delete[] p;
    }
};

template<typename T, typename Deleter = default_deleter<T>>
class MyUniquePtr
{
public:
    using pointer = T*;

    MyUniquePtr() = default;
    MyUniquePtr(pointer p) : _p{p} {}
    ~MyUniquePtr() {
        Deleter()(_p);
    }
    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr(MyUniquePtr&& uptr) noexcept : _p{uptr._p}{
        uptr._p = nullptr;
    }
    MyUniquePtr& operator=(MyUniquePtr&& uptr) noexcept {
        reset(uptr.release());
        return *this;
    }
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    T& operator*() noexcept {
        return *_p;
    }

    T* operator->() noexcept {
        return _p;
    }
    explicit operator bool() const noexcept { return _p; }

    void swap(MyUniquePtr& uptr) noexcept {
        std::exchange(_p, uptr._p);
    }

    void reset(pointer p = nullptr) noexcept {
        Deleter()(_p);
        _p = p;
    }
    pointer get() const noexcept {
        return _p;
    }
    pointer release() {
        pointer p = _p;
        _p = nullptr;
        return p;
    }

private:
    pointer _p{};
};

