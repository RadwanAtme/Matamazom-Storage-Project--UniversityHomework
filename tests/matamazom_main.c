#include "test_utilities.h"
#include "matamazom_tests.c"
#include "../matamazom.c"
#include "../amount_set.c"
#include "../matamazom_print.c"


int main()
{
    RUN_TEST(testCreate);
    RUN_TEST(testDestroy);
    RUN_TEST(testModifyProducts);
    RUN_TEST(testModifyOrders);
   // RUN_TEST(testPrintInventory);
   // RUN_TEST(testPrintOrder);
   // RUN_TEST(testPrintBestSelling);
   // RUN_TEST(testPrintFiltered);
    return 0;
}
