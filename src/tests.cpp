#include <gtest/gtest.h>

#include <string>

#include "variant.hpp"

TEST(Variant, DefaultConstructor)
{
    Variant<int, double, std::string> v;

    EXPECT_EQ(get<int>(v), 0);
    EXPECT_THROW(get<double>(v), VariantAccessError);
    EXPECT_THROW(get<std::string>(v), VariantAccessError);

    EXPECT_EQ(get<0>(v), 0);
    EXPECT_THROW(get<1>(v), VariantAccessError);
    EXPECT_THROW(get<2>(v), VariantAccessError);
}

TEST(Variant, VariadicConstructor)
{
    Variant<int, double, std::string> v = std::string("hi");

    EXPECT_EQ(get<std::string>(v), "hi");
    EXPECT_THROW(get<int>(v), VariantAccessError);
    EXPECT_THROW(get<double>(v), VariantAccessError);

    EXPECT_EQ(get<2>(v), "hi");
    EXPECT_THROW(get<0>(v), VariantAccessError);
    EXPECT_THROW(get<1>(v), VariantAccessError);
}

TEST(Variant, VariadicAssignment)
{
    Variant<int, double, std::string> v = std::string("hi");
    EXPECT_EQ(get<std::string>(v), "hi");

    v = 2.26;
    EXPECT_EQ(get<double>(v), 2.26);
    EXPECT_THROW(get<std::string>(v), VariantAccessError);
    EXPECT_THROW(get<int>(v), VariantAccessError);
}

TEST(Variant, CommonAssignment)
{
    Variant<int, double, std::string> v1 = 10;
    Variant<int, double, std::string> v2 = std::string("hi");
    v1 = v2;
    EXPECT_EQ(get<std::string>(v1), "hi");
    v2 = 42;
    EXPECT_EQ(get<std::string>(v1), "hi");
    EXPECT_EQ(get<int>(v2), 42);
}

TEST(Variant, GetIf)
{
    Variant<int, double, std::string> v = std::string("hi");

    EXPECT_EQ(*getIf<std::string>(v), "hi");
    EXPECT_EQ(getIf<int>(v), nullptr);
    EXPECT_EQ(getIf<double>(v), nullptr);

    EXPECT_EQ(*getIf<2>(v), "hi");
    EXPECT_EQ(getIf<0>(v), nullptr);
    EXPECT_EQ(getIf<1>(v), nullptr);
}
