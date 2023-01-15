#pragma once

#include "error.hpp"

#include <cstddef>

#include <exception>

namespace Utils
{
    template <class...>
    union Variadic {};

    template <class First, class... Rest>
    union Variadic<First, Rest...>
    {
        First first;
        Variadic<Rest...> rest;

        template <size_t Idx>
        auto& get() const
        {
            if constexpr (Idx == 0) {
                return first;
            } else {
                return rest.template get<Idx - 1>();
            }
        }

        template <class T>
        void set(const T &value)
        {
            if constexpr (std::is_same_v<T, First>) {
                first = value;
            } else {
                rest.set(value);
            }
        }
    }; 

    template <class T, class... Types>
    struct IndexByType
    {
        static const size_t value = 0;
    };

    template <class... Types>
    constexpr size_t IndexByTypeV = IndexByType<Types...>::value;

    template <class T, class First, class... Rest>
    struct IndexByType<T, First, Rest...>
    {
        static constexpr size_t value = std::is_same_v<T, First> ? 0 : IndexByType<T, Rest...>::value + 1;
    };
}

template <class... Types>
class Variant 
{
public:
    template <size_t Idx, class... Types_>
    friend auto& get(Variant<Types_...>&);

    template <class T, class... Types_>
    friend auto& get(Variant<Types_...>&);

    Variant() {}

    template <class T>
    Variant(const T& value)
    {
        idx_ = Utils::IndexByTypeV<T, Types...>;
        body_.set(value);
    }

private:
public:
    Utils::Variadic<Types...> body_;
    size_t idx_;
};

template <size_t Idx, class... Types>
auto& get(Variant<Types...> &v)
{
    if (Idx != v.idx_) {
        throw VariantAccessError();
    }
    return v.body_.template get<Idx>();
}

template <class T, class... Types>
auto& get(Variant<Types...> &v)
{
    return get<Utils::IndexByTypeV<T, Types...>>(v);
}
