#include <gtest/gtest.h>
#include "kmp.h"

TEST(ZFunc, test01) {
    TString s = {"a", "a", "b", "c", "a", "a", "b", "x", "a", "a", "z"};
    std::vector<int> res = ZFunction(s);
    EXPECT_EQ(res[4], 3);
    EXPECT_EQ(res[5], 1);
    EXPECT_EQ(res[6], 0);
    EXPECT_EQ(res[7], 0);
    EXPECT_EQ(res[8], 2);
}

TEST(ZFunc, test02) {
    TString s = {"cat", "dog", "cat", "dog", "cat"};
    std::vector<int> res = ZFunction(s);
    EXPECT_EQ(res[1], 0);
    EXPECT_EQ(res[2], 3);
    EXPECT_EQ(res[3], 0);
    EXPECT_EQ(res[4], 1);
}
