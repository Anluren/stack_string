#include <gtest/gtest.h>
#include <stack_string.hpp>

using namespace stack_string;

TEST(StackStringTest, BasicAppendAndSize) {
    StackString<32> s;
    s.append("Hello");
    s.append(", World!");
    EXPECT_EQ(s.size(), 13u);
    EXPECT_STREQ(s.c_str(), "Hello, World!");
}

TEST(StackStringTest, TruncationOnOverflow) {
    StackString<8> s;
    s.append("1234567890");
    // Should fit 7 chars + null terminator
    EXPECT_EQ(s.size(), 7u);
    EXPECT_EQ(std::string_view(s.c_str()).substr(0, 7), "1234567");
    EXPECT_EQ(s.c_str()[7], '\0');
}

TEST(StackStringTest, StreamStyleAppend) {
    StackString<32> s;
    s << "ID: " << 42;
    EXPECT_STREQ(s.c_str(), "ID: 42");
}

TEST(StackStringTest, VariadicConstructor) {
    StackString<32> s("A", 1, ", B", 2);
    EXPECT_STREQ(s.c_str(), "A1, B2");
}

TEST(StackStringTest, ImplicitConversion) {
    StackString<16> s("abc");
    std::string_view sv = s;
    EXPECT_EQ(sv, "abc");
    const char* cstr = s;
    EXPECT_STREQ(cstr, "abc");
}
