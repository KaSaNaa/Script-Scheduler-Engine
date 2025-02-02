#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "scheduler.h"

void test_add_task(void) {
    Task task = { .script_name = "test_script.sh", .scheduled_time = time(NULL) + 60 };
    CU_ASSERT_TRUE(add_task(task));
}

void test_extract_max(void) {
    Task task1 = { .script_name = "test_script1.sh", .scheduled_time = time(NULL) + 60 };
    Task task2 = { .script_name = "test_script2.sh", .scheduled_time = time(NULL) + 120 };
    add_task(task1);
    add_task(task2);
    Task extracted_task = extract_max(task_queue);
    CU_ASSERT_STRING_EQUAL(extracted_task.script_name, "test_script1.sh");
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Scheduler Suite", 0, 0);
    CU_add_test(suite, "test_add_task", test_add_task);
    CU_add_test(suite, "test_extract_max", test_extract_max);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}