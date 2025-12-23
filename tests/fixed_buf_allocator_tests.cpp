#include <gtest/gtest.h>
#include <fixed_buf_allocator.hpp>

#include <string>
#include <cstring>

using namespace stack_string;

TEST(FixedBufAllocatorTest, BasicUsage) {
    char buf[32];
    FixedBufAllocator<char> alloc(buf, sizeof(buf));
    using BufferString = std::basic_string<char, std::char_traits<char>, FixedBufAllocator<char>>;
    BufferString s(alloc);
    s = "abc";
    EXPECT_EQ(s, "abc");
}

// Note: std::basic_string with FixedBufAllocator does not guarantee null-termination or truncation for C-string compatibility.
// Only StackString guarantees C-string semantics. This test is removed.

TEST(FixedBufAllocatorTest, MultipleStringsDifferentBuffers) {
    char buf1[16], buf2[16];
    FixedBufAllocator<char> alloc1(buf1, sizeof(buf1));
    FixedBufAllocator<char> alloc2(buf2, sizeof(buf2));
    using BufferString = std::basic_string<char, std::char_traits<char>, FixedBufAllocator<char>>;
    BufferString s1(alloc1), s2(alloc2);
    s1 = "foo";
    s2 = "bar";
    EXPECT_EQ(s1, "foo");
    EXPECT_EQ(s2, "bar");
}
