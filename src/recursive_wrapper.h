#pragma once

#include <boost/checked_delete.hpp>

namespace cx {

template <typename T>
class rec
{
public:
    typedef T type;

private:
    T* p_;
    constexpr void assign(const T& rhs);

public:
    ~rec();
    constexpr rec();

    constexpr rec(const rec& operand);
    constexpr rec(const T& operand);

    constexpr rec(rec&& operand);
    constexpr rec(T&& operand);

    constexpr rec& operator=(const rec& rhs) {
        assign(rhs.get());
        return *this;
    }

    constexpr rec& operator=(const T& rhs) {
        assign(rhs);
        return *this;
    }

    constexpr void swap(rec& operand) noexcept {
        T* temp = operand.p_;
        operand.p_ = p_;
        p_ = temp;
    }

    constexpr rec& operator=(rec&& rhs) noexcept {
        swap(rhs);
        return *this;
    }

    constexpr rec& operator=(T&& rhs) {
        get() = std::move(rhs);
        return *this;
    }

    constexpr T& get() noexcept { return *p_; }
    constexpr const T& get() const noexcept { return *p_; }

    constexpr T& operator*() noexcept { return get(); }
    constexpr const T& operator*() const noexcept { return get(); }

    constexpr T* operator->() noexcept { return std::addressof(get()); }
    constexpr const T* operator->() const noexcept { return std::addressof(get()); }
};

template <typename T>
rec<T>::~rec() {
    boost::checked_delete(p_);
}

template <typename T>
constexpr rec<T>::rec()
    : p_(new T)
{
}

template <typename T>
constexpr rec<T>::rec(const rec& operand)
    : p_(new T(operand.get()))
{
}

template <typename T>
constexpr rec<T>::rec(const T& operand)
    : p_(new T(operand))
{
}

template <typename T>
constexpr rec<T>::rec(rec&& operand)
    : p_(new T(std::move(operand.get())))
{
}

template <typename T>
constexpr rec<T>::rec(T&& operand)
    : p_(new T(std::move(operand)))
{
}

template <typename T>
constexpr void rec<T>::assign(const T& rhs) {
    this->get() = rhs;
}

template <typename T>
constexpr inline void swap(rec<T>& lhs, rec<T>& rhs) noexcept {
    lhs.swap(rhs);
}

template<class T>
constexpr bool operator==(rec<T> const& l, rec<T> const& r) {
    return l.get() == r.get();
}

}
