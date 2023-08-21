#include "munit/munit.h"

extern MunitTest dataReadWriteTests[];
extern MunitTest trainerTests[];

static const MunitSuite test_suite = {
  (char*) "Octet/",
  NULL,
  (MunitSuite[]) {
    //{ (char*) "Preparer", dataReadWriteTests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { (char*) "Trainer", trainerTests, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE }
  },
  1,
  MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}
