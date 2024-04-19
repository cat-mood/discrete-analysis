#include <gtest/gtest.h>
#include "patricia.h"

TEST(patricia_test, modifier01) {
    TPatriciaTrie p;
    p.Insert(TPair("a", 1));
    EXPECT_EQ(p.Find("a").value, 1);
}

TEST(patricia_test, modifier02) {
    TPatriciaTrie p;
    p.Insert(TPair("a", 1));
    EXPECT_ANY_THROW(p.Insert(TPair("A", 2)));
}

TEST(patricia_test, modifier03) {
    TPatriciaTrie p;
    p.Insert(TPair("abc", 10));
    EXPECT_ANY_THROW(p.Insert(TPair("abc", 20)));
}

TEST(patricia_test, modifier04) {
    TPatriciaTrie p;
    p.Insert(TPair("ab", 10));
    p.Insert(TPair("abc", 20));
    EXPECT_EQ(p.Find("ab").value, 10);
    EXPECT_EQ(p.Find("abc").value, 20);
}

TEST(patricia_test, modifier05) {
    TPatriciaTrie p;
    p.Insert(TPair("ab", 10));
    p.Insert(TPair("abc", 20));
    EXPECT_EQ(p.Find("Ab").value, 10);
    EXPECT_EQ(p.Find("ABC").value, 20);
    EXPECT_ANY_THROW(p.Find("abcd"));
}

// TEST(patricia_test, modifier06) {
//     TPatriciaTrie p;
//     p.Insert(TPair("ab", 10));
//     p.Insert(TPair("abc", 20));
//     p.Erase("ab");
//     EXPECT_ANY_THROW(p.Find("ab"));
//     EXPECT_EQ(p.Find("abc").value, 20);
// }

// TEST(patricia_test, modifier07) {
//     TPatriciaTrie p;
//     p.Insert(TPair("ab", 10));
//     p.Insert(TPair("abc", 20));
//     p.Erase("ABc");
//     EXPECT_EQ(p.Find("ab").value, 10);
//     EXPECT_ANY_THROW(p.Find("aBc"));
// }

// TEST(patricia_test, modifier08) {
//     TPatriciaTrie p;
//     EXPECT_ANY_THROW(p.Erase("ab"));
// }

// TEST(patricia_test, modifier09) {
//     TPatriciaTrie p;
//     EXPECT_ANY_THROW(p.Find("ab"));
// }

// TEST(patricia_test, modifier10) {
//     TPatriciaTrie p;
//     p.Insert(TPair("ab", 10));
//     p.Insert(TPair("abc", 20));
//     p.Insert(TPair("a", 30));
//     p.Insert(TPair("b", 40));
//     p.Insert(TPair("bac", 50));
//     p.Erase("a");
//     EXPECT_ANY_THROW(p.Find("a"));
//     EXPECT_EQ(p.Find("ab").value, 10);
//     EXPECT_EQ(p.Find("abc").value, 20);
// }

// TEST(patricia_test, modifier11) {
//     TPatriciaTrie p;
//     p.Insert(TPair("ab", 10));
//     p.Insert(TPair("abc", 20));
//     p.Insert(TPair("a", 30));
//     p.Erase("a");
//     p.Erase("ab");
//     p.Erase("abc");
//     p.Insert(TPair("a", 30));
//     EXPECT_EQ(p.Find("a").value, 30);
// }

TEST(patricia_test, modifier12) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 20; ++i) {
        p.Insert(TPair(s + static_cast<char>('a' + i), i));
    }
    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(p.Find(s + static_cast<char>('a' + i)).value, i);
    }
}

TEST(patricia_test, modifier13) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 10; ++i) {
        p.Insert(TPair(s + static_cast<char>('a' + i), i));
        p.Insert(TPair(s + static_cast<char>('z' - i), i));
    }
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(p.Find(s + static_cast<char>('a' + i)).value, i);
        EXPECT_EQ(p.Find(s + static_cast<char>('z' - i)).value, i);
    }
}
