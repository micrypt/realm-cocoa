/* @@Example: ex_objc_table_typed_intro @@ */
#import <Realm/Realm.h>
#import "people.h"

/*
 The classes People, PeopleQuery, PeopleView, and PeopleRow are declared 
 (interfaces are generated) in people.h as

 REALM_TABLE_DEF_3(People,
                     Name,  String,
                     Age,   Int,
                     Hired, Bool)
 
 and in people.m you must have
 
 REALM_TABLE_IMPL_3(People,
                      Name, String,
                      Age,  Int,
                      Hired, Bool)
 
in order to generate the implementation of the classes.
*/

void ex_objc_table_typed_intro()
{
    // Create a new table of the type defined above
    People *table = [[People alloc] init];
    
    // Append three rows
    [table addRow:@{@"Name":@"Brian",  @"Age":@10,  @"Hired":@NO}];
    [table addRow:@{@"Name":@"Sofie",  @"Age":@40,  @"Hired":@YES}];
    [table addRow:@{@"Name":@"Jesper", @"Age":@200, @"Hired":@NO}];
    NSLog(@"The size of the table is now %zd", table.rowCount);

    for (PeopleRow *row in table) {
        NSLog(@"Name: %@ Age: %lli", row.Name, row.Age);
    }
    
    NSLog(@"Insert a new row");
    [table insertRow:@{@"Name":@"Sam", @"Age":@30, @"Hired":@YES}
             atIndex:1];
    
    for (PeopleRow *row in table) {
        NSLog(@"Name: %@ Age: %lli", row.Name, row.Age);
    }
    
    PeopleRow *row2 = [table rowAtLastIndex];
    if (row2 != nil)
        NSLog(@"Last row");
    
    PeopleRow *row3 = [table rowAtIndex:42];
    if (row3 == nil)
        NSLog(@"Index out of range.");
}
/* @@EndExample@@ */