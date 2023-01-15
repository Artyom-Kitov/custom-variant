#include "variant.hpp"

#include <exception>
#include <iostream>
#include <variant>

int main()
{
    Variant<int, double> v = 23.22;
    std::cout << get<double>(v) << std::endl;
    std::cout << get<int>(v) << std::endl;
    std::cout << get<0>(v) << std::endl;
    std::cout << get<1>(v) << std::endl;


    return 0;
}
