#include "variant.hpp"

#include <exception>
#include <iostream>
#include <variant>

int main()
{
    // Variant<int, double, bool> v = 23.22;

    // try {
    //     std::cout << get<int>(v) << std::endl;
    // } catch (VariantAccessError &err) {
    //     std::cout << err.what() << std::endl;
    // }

    // std::cout << get<double>(v) << std::endl;

    // try {
    //     std::cout << get<0>(v) << std::endl;
    // } catch (VariantAccessError &err) {
    //     std::cout << err.what() << std::endl;
    // }

    // std::cout << get<1>(v) << std::endl;

    std::variant<int, double> v;
    // std::cout << std::get<double>(v) << std::endl;

    return 0;
}
