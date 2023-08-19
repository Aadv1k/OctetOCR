#include <octet.h>
#include "munit/munit.h"

static MunitResult isProcessed_fromRawData_toFile(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    return MUNIT_OK;
}

MunitTest dataReadWriteTests[] = {
  { (char*) "/isProcessed_fromRawData_toFile", isProcessed_fromRawData_toFile, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
