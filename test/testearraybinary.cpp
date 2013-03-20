#include <UnitTest++.h>
#include <tightdb/array_binary.hpp>

using namespace tightdb;

struct db_setup_binary {
    static ArrayBinary c;
};

ArrayBinary db_setup_binary::c;

TEST_FIXTURE(db_setup_binary, ArrayBinaryMultiEmpty)
{
    c.add("", 0);
    c.add("", 0);
    c.add("", 0);
    c.add(NULL, 0);
    c.add(NULL, 0);
    c.add(NULL, 0);

    CHECK_EQUAL(6, c.size());

    CHECK_EQUAL(0, c.get(0).len);
    CHECK_EQUAL(0, c.get(1).len);
    CHECK_EQUAL(0, c.get(2).len);
    CHECK_EQUAL(0, c.get(3).len);
    CHECK_EQUAL(0, c.get(4).len);
    CHECK_EQUAL(0, c.get(5).len);
}

TEST_FIXTURE(db_setup_binary, ArrayBinarySet)
{
    c.Set(0, "hey", 4);

    CHECK_EQUAL(6, c.size());

    CHECK_EQUAL("hey", c.get(0).pointer);
    CHECK_EQUAL(4, c.get(0).len);
    CHECK_EQUAL(0, c.get(1).len);
    CHECK_EQUAL(0, c.get(2).len);
    CHECK_EQUAL(0, c.get(3).len);
    CHECK_EQUAL(0, c.get(4).len);
    CHECK_EQUAL(0, c.get(5).len);
}

TEST_FIXTURE(db_setup_binary, ArrayBinaryAdd)
{
    c.Clear();
    CHECK_EQUAL(0, c.size());

    c.add("abc", 4);
    CHECK_EQUAL("abc", c.get(0).pointer); // single
    CHECK_EQUAL(4, c.get(0).len);
    CHECK_EQUAL(1, c.size());

    c.add("defg", 5); //non-empty
    CHECK_EQUAL("abc", c.get(0).pointer);
    CHECK_EQUAL("defg", c.get(1).pointer);
    CHECK_EQUAL(4, c.get(0).len);
    CHECK_EQUAL(5, c.get(1).len);
    CHECK_EQUAL(2, c.size());
}

TEST_FIXTURE(db_setup_binary, ArrayBinarySet2)
{
    // {shrink, grow} x {first, middle, last, single}
    c.Clear();

    c.add("abc", 4);
    c.Set(0, "de", 3); // shrink single
    CHECK_EQUAL("de", c.get(0).pointer);
    CHECK_EQUAL(1, c.size());

    c.Set(0, "abcd", 5); // grow single
    CHECK_EQUAL("abcd", c.get(0).pointer);
    CHECK_EQUAL(1, c.size());

    c.add("efg", 4);
    CHECK_EQUAL("abcd", c.get(0).pointer);
    CHECK_EQUAL("efg", c.get(1).pointer);
    CHECK_EQUAL(2, c.size());

    c.Set(1, "hi", 3); // shrink last
    CHECK_EQUAL("abcd", c.get(0).pointer);
    CHECK_EQUAL("hi", c.get(1).pointer);
    CHECK_EQUAL(2, c.size());

    c.Set(1, "jklmno", 7); // grow last
    CHECK_EQUAL("abcd", c.get(0).pointer);
    CHECK_EQUAL("jklmno", c.get(1).pointer);
    CHECK_EQUAL(2, c.size());

    c.add("pq", 3);
    c.Set(1, "efghijkl", 9); // grow middle
    CHECK_EQUAL("abcd", c.get(0).pointer);
    CHECK_EQUAL("efghijkl", c.get(1).pointer);
    CHECK_EQUAL("pq", c.get(2).pointer);
    CHECK_EQUAL(3, c.size());

    c.Set(1, "x", 2); // shrink middle
    CHECK_EQUAL("abcd", c.get(0).pointer);
    CHECK_EQUAL("x", c.get(1).pointer);
    CHECK_EQUAL("pq", c.get(2).pointer);
    CHECK_EQUAL(3, c.size());

    c.Set(0, "qwertyuio", 10); // grow first
    CHECK_EQUAL("qwertyuio", c.get(0).pointer);
    CHECK_EQUAL("x", c.get(1).pointer);
    CHECK_EQUAL("pq", c.get(2).pointer);
    CHECK_EQUAL(3, c.size());

    c.Set(0, "mno", 4); // shrink first
    CHECK_EQUAL("mno", c.get(0).pointer);
    CHECK_EQUAL("x", c.get(1).pointer);
    CHECK_EQUAL("pq", c.get(2).pointer);
    CHECK_EQUAL(3, c.size());
}

TEST_FIXTURE(db_setup_binary, ArrayBinaryInsert)
{
    c.Clear();

    c.Insert(0, "abc", 4); // single
    CHECK_EQUAL("abc", c.get(0).pointer);
    CHECK_EQUAL(1, c.size());

    c.Insert(1, "d", 2); // end
    CHECK_EQUAL("abc", c.get(0).pointer);
    CHECK_EQUAL("d", c.get(1).pointer);
    CHECK_EQUAL(2, c.size());

    c.Insert(2, "ef", 3); // end
    CHECK_EQUAL("abc", c.get(0).pointer);
    CHECK_EQUAL("d", c.get(1).pointer);
    CHECK_EQUAL("ef", c.get(2).pointer);
    CHECK_EQUAL(3, c.size());

    c.Insert(1, "ghij", 5); // middle
    CHECK_EQUAL("abc", c.get(0).pointer);
    CHECK_EQUAL("ghij", c.get(1).pointer);
    CHECK_EQUAL("d", c.get(2).pointer);
    CHECK_EQUAL("ef", c.get(3).pointer);
    CHECK_EQUAL(4, c.size());

    c.Insert(0, "klmno", 6); // first
    CHECK_EQUAL("klmno", c.get(0).pointer);
    CHECK_EQUAL("abc", c.get(1).pointer);
    CHECK_EQUAL("ghij", c.get(2).pointer);
    CHECK_EQUAL("d", c.get(3).pointer);
    CHECK_EQUAL("ef", c.get(4).pointer);
    CHECK_EQUAL(5, c.size());
}

TEST_FIXTURE(db_setup_binary, ArrayBinaryDelete)
{
    c.Clear();

    c.add("a", 2);
    c.add("bc", 3);
    c.add("def", 4);
    c.add("ghij", 5);
    c.add("klmno", 6);

    c.Delete(0); // first
    CHECK_EQUAL("bc", c.get(0).pointer);
    CHECK_EQUAL("def", c.get(1).pointer);
    CHECK_EQUAL("ghij", c.get(2).pointer);
    CHECK_EQUAL("klmno", c.get(3).pointer);
    CHECK_EQUAL(4, c.size());

    c.Delete(3); // last
    CHECK_EQUAL("bc", c.get(0).pointer);
    CHECK_EQUAL("def", c.get(1).pointer);
    CHECK_EQUAL("ghij", c.get(2).pointer);
    CHECK_EQUAL(3, c.size());

    c.Delete(1); // middle
    CHECK_EQUAL("bc", c.get(0).pointer);
    CHECK_EQUAL("ghij", c.get(1).pointer);
    CHECK_EQUAL(2, c.size());

    c.Delete(0); // single
    CHECK_EQUAL("ghij", c.get(0).pointer);
    CHECK_EQUAL(1, c.size());

    c.Delete(0); // all
    CHECK_EQUAL(0, c.size());
    CHECK(c.is_empty());
}

TEST_FIXTURE(db_setup_binary, ArrayBinary_Destroy)
{
    // clean up (ALWAYS PUT THIS LAST)
    c.Destroy();
}