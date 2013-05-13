// @@Example: ex_cpp_group_constructor_plain @@
// @@Fold@@
#include <tightdb.hpp>

using namespace tightdb;

// @@EndFold@@
TIGHTDB_TABLE_2(PeopleTable,
                name, String,
                age, Int)

int main()
{
    Group g;
    PeopleTable::Ref table = g.get_table<PeopleTable>("people");

    table->add("Mary", 14);
    table->add("Joe", 17);
    table->add("Jack", 22);

    g.write("people.tightdb");
}
// @@EndExample@@