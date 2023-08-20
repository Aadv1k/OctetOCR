#include "munit/munit.h"

extern MunitTest dataReadWriteTests[];

static const MunitSuite test_suite = {
  (char*) "Read_Write",
  dataReadWriteTests,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}
