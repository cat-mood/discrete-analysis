#include "binary_string.h"
#include <gtest/gtest.h>

TEST(binary_string_test, bitdifftest01) {
    std::string a("a");   // 00001
    std::string b("b");   // 00010
    EXPECT_EQ(BitDifference(a, b), 3);
}

TEST(binary_string_test, bitdifftest02) {
    std::string a("abc");
    std::string b("abc");
    EXPECT_EQ(BitDifference(a, b), -1);
}

TEST(binary_string_test, bitdifftest03) {
    std::string a("abc");     // 00001 00010 00011
    std::string b("abd");     // 00010 00010 00100
    EXPECT_EQ(BitDifference(a, b), 12);
}

TEST(binary_string_test, bitdifftest04) {
    std::string a("abc");     // 00001 00010 00011    
    std::string b("aad");     // 00001 00001 00100
    EXPECT_EQ(BitDifference(a, b), 8);
}

TEST(binary_string_test, bitdifftest05) {
    std::string a(""); 
    std::string b("");
    EXPECT_EQ(BitDifference(a, b), -1);
}

TEST(binary_string_test, bitdifftest06) {
    std::string a("a");     // 00001
    std::string b("");      // 00000
    EXPECT_EQ(BitDifference(a, b), 4);
}

TEST(binary_string_test, bitdifftest07) {
    std::string a("ab");    // 00001 00010
    std::string b("a");     // 00001
    EXPECT_EQ(BitDifference(a, b), 8);
}

TEST(binary_string_test, bitdifftest08) {
    std::string a("Ab");     // 00001 00010
    std::string b("a");      // 00001 00000
    EXPECT_EQ(BitDifference(a, b), 8);
}

TEST(binary_string_test, bitdifftest09) {
    std::string a("ABc");     // 00001 00010 00011
    std::string b("abd");     // 00001 00010 00100
    EXPECT_EQ(BitDifference(a, b), 12);
}

TEST(binary_string_test, bitindex01) {
    std::string a("a");     // 00001
    EXPECT_EQ(GetBitByIndex(a, 0), 0);
}

TEST(binary_string_test, bitindex02) {
    std::string a("z");     // 11010
    EXPECT_EQ(GetBitByIndex(a, 1), 1);
}

TEST(binary_string_test, bitindex03) {
    std::string a("Z");
    EXPECT_EQ(GetBitByIndex(a, 1), 1);
}

TEST(binary_string_test, bitindex04) {
    std::string a("AbC");   // 00001 00010 00011
    EXPECT_EQ(GetBitByIndex(a, 9), 0);
}

TEST(binary_string_test, bitindex05) {
    std::string a("a");
    EXPECT_ANY_THROW(GetBitByIndex(a, -1));
}

TEST(binary_string_test, bitindex06) {
    std::string a("a");
    EXPECT_EQ(GetBitByIndex(a, 5), 0);
}
