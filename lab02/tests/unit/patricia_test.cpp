#include <gtest/gtest.h>
#include "patricia.h"

TEST(patricia_test, modifier01) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    EXPECT_EQ(p.Find("a").value, 1);
}

TEST(patricia_test, modifier02) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    EXPECT_ANY_THROW(p.Insert({"A", 2}));
}

TEST(patricia_test, modifier03) {
    TPatriciaTrie p;
    p.Insert({"abc", 10});
    EXPECT_ANY_THROW(p.Insert({"abc", 20}));
}

TEST(patricia_test, modifier04) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    EXPECT_EQ(p.Find("ab").value, 10);
    EXPECT_EQ(p.Find("abc").value, 20);
}

TEST(patricia_test, modifier05) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    EXPECT_EQ(p.Find("Ab").value, 10);
    EXPECT_EQ(p.Find("ABC").value, 20);
    EXPECT_ANY_THROW(p.Find("abcd"));
}

TEST(patricia_test, modifier06) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    p.Erase("ab");
    EXPECT_ANY_THROW(p.Find("ab"));
    EXPECT_EQ(p.Find("abc").value, 20);
}

TEST(patricia_test, modifier07) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    p.Erase("ABc");
    EXPECT_EQ(p.Find("ab").value, 10);
    EXPECT_ANY_THROW(p.Find("aBc"));
}

TEST(patricia_test, modifier08) {
    TPatriciaTrie p;
    EXPECT_ANY_THROW(p.Erase("ab"));
}

TEST(patricia_test, modifier09) {
    TPatriciaTrie p;
    EXPECT_ANY_THROW(p.Find("ab"));
}

TEST(patricia_test, modifier10) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    p.Insert({"a", 30});
    p.Insert({"b", 40});
    p.Insert({"bac", 50});
    p.Erase("a");
    EXPECT_ANY_THROW(p.Find("a"));
    EXPECT_EQ(p.Find("ab").value, 10);
    EXPECT_EQ(p.Find("abc").value, 20);
}

TEST(patricia_test, modifier11) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    p.Insert({"a", 30});
    p.Erase("a");
    p.Erase("ab");
    p.Erase("abc");
    p.Insert({"a", 30});
    EXPECT_EQ(p.Find("a").value, 30);
}

TEST(patricia_test, modifier12) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 20; ++i) {
        p.Insert({s + static_cast<char>('a' + i), static_cast<uint64_t>(i)});
    }
    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(p.Find(s + static_cast<char>('a' + i)).value, static_cast<uint64_t>(i));
    }
}

TEST(patricia_test, modifier13) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 10; ++i) {
        p.Insert({s + static_cast<char>('a' + i), static_cast<uint64_t>(i)});
        p.Insert({s + static_cast<char>('z' - i), static_cast<uint64_t>(i)});
    }
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(p.Find(s + static_cast<char>('a' + i)).value, i);
        EXPECT_EQ(p.Find(s + static_cast<char>('z' - i)).value, i);
    }
}

TEST(patricia_test, modifier14) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 10; ++i) {
        p.Insert({s + static_cast<char>('a' + i), static_cast<uint64_t>(i)});
        p.Insert({s + static_cast<char>('z' - i), static_cast<uint64_t>(i)});
    }
    for (int i = 0; i < 10; ++i) {
        p.Erase(s + static_cast<char>('a' + i));
        EXPECT_EQ(p.Find(s + static_cast<char>('z' - i)).value, i);
        p.Erase(s + static_cast<char>('z' - i));
    }
    EXPECT_ANY_THROW(p.Find("a"));
}
