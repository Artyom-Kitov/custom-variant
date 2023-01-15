#pragma once

#include <cstddef>

namespace Utils
{
    template <class...>
    union Variadic
    {
        template <size_t Idx, class T>
        T& get()
        {
            // anything
        }
    };

    template <class First, class... Rest>
    union Variadic<First, Rest...>
    {
        First first;
        Variadic<Rest...> rest;

        template <size_t Idx, class T>
        T& get()
        {
            if constexpr (Idx == 0) {
                return first;
            } else {
                return rest.template get<Idx - 1>();
            }
        }
    }; 
}

template <class... Types>
class Variant
{
    template <size_t Idx, class T, class... Types_>
    friend T& get(Variant<Types_...>&);

private:
    Utils::Variadic<Types...> body_;
    size_t idx_;

};

template <size_t Idx, class T, class... Types>
T& get(Variant<Types...> &v)
{
    if (Idx != v.idx_) {
        // throw ...
    }
    return v.body_.template get<Idx>();
}
