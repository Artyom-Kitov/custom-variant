#pragma once

#include "error.hpp"

#include <cstddef>

#include <exception>

namespace Utils
{
    template <class T, class ...Types>
    struct IndexByType
    {
        static const size_t value = 0;
    };

    template <class T, class First, class ...Rest>
    struct IndexByType<T, First, Rest...>
    {
        static constexpr size_t value = std::is_same<T, First>::value ? 0 : IndexByType<T, Rest...>::value + 1;
    };

    template <size_t Idx, class First, class ...Rest>
    struct TypeByIndex
    {
        using Type = typename TypeByIndex<Idx - 1, Rest...>::Type;
    };

    template <class First, class ...Rest>
    struct TypeByIndex<0, First, Rest...>
    {
        using Type = First; 
    };

    template <size_t Idx, class First, class ...Rest>
    struct CopyByIndex
    {
        void* operator()(const size_t idx, void *ptr)
        {
            if (Idx == idx) {
                First *ret = new First(*static_cast<First*>(ptr));
                return static_cast<void*>(ret);
            } else {
                return CopyByIndex<Idx + 1, Rest...>()(idx, ptr);
            }
        }
    };

    template <size_t Idx, class T>
    struct CopyByIndex<Idx, T>
    {
        void* operator()(const size_t idx, void *ptr)
        {
            T *ret = new T(*static_cast<T*>(ptr));
            return static_cast<void*>(ret);
        }
    };

    template <size_t Idx, class First, class ...Rest>
    struct Deleter
    {
        void operator()(const size_t idx, void *ptr)
        {
            if (Idx == idx) {
                delete static_cast<First*>(ptr);
            } else {
                Deleter<Idx + 1, Rest...>()(idx, ptr);
            }
        }
    }; 

    template <size_t Idx, class T>
    struct Deleter<Idx, T>
    {
        void operator()(const size_t idx, void *ptr)
        {
            if (Idx == idx) {
                delete static_cast<T*>(ptr);
            }
        }
    };
}

template <class First, class ...Rest>
class Variant 
{
public:
    template <size_t Idx, class ...Types>
    friend auto& get(const Variant<Types...>&);

    template <class T, class ...Types>
    friend T& get(const Variant<Types...>&);

    template <size_t Idx, class ...Types>
    friend typename Utils::TypeByIndex<Idx, Types...>::Type* getIf(const Variant<Types...>&);

    template <class T, class ...Types>
    friend T* getIf(const Variant<Types...>&);

    Variant() :
        body_(new First())
        , idx_(0)
    {}

    template <class T>
    Variant(const T& value) :
        idx_(Utils::IndexByType<T, First, Rest...>::value)
        , body_(static_cast<void*>(new T(value)))
    {}

    template <class T>
    Variant(T &&value) :
        idx_(Utils::IndexByType<T, First, Rest...>::value)
        , body_(static_cast<void*>(new T(value)))
    {}

    ~Variant()
    {
        Utils::Deleter<0, First, Rest...>()(idx_, body_);
    }

    template <class T>
    Variant& operator=(const T &value)
    {
        Utils::Deleter<0, First, Rest...>()(idx_, body_);
        idx_ = Utils::IndexByType<T, First, Rest...>::value;
        body_ = static_cast<void*>(new T(value));
        return *this;
    }

    Variant& operator=(const Variant<First, Rest...> &other)
    {
        if (this->body_ == other.body_) {
            return *this;
        }
        Utils::Deleter<0, First, Rest...>()(idx_, body_);
        idx_ = other.idx_;
        body_ = Utils::CopyByIndex<0, First, Rest...>()(idx_, other.body_);
        return *this;
    }

private:
    void *body_;
    size_t idx_;
};

template <size_t Idx, class ...Types>
auto& get(const Variant<Types...> &v)
{
    if (Idx != v.idx_) {
        throw VariantAccessError();
    }
    return *(static_cast<typename Utils::TypeByIndex<Idx, Types...>::Type*>(v.body_));
}

template <class T, class ...Types>
T& get(const Variant<Types...> &v)
{
    return get<Utils::IndexByType<T, Types...>::value>(v);
}

template <size_t Idx, class ...Types>
typename Utils::TypeByIndex<Idx, Types...>::Type* getIf(const Variant<Types...> &v)
{
    if (Idx != v.idx_) {
        return nullptr;
    }
    return static_cast<typename Utils::TypeByIndex<Idx, Types...>::Type*>(v.body_);
}

template <class T, class ...Types>
T* getIf(const Variant<Types...> &v)
{
    return getIf<Utils::IndexByType<T, Types...>::value>(v);
}
