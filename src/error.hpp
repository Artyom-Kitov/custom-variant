#pragma once

#include <exception>

class VariantAccessError : public std::exception
{
    const char* what() const noexcept override
    {
        return "variant access error";
    }
};
