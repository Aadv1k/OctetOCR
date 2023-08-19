#include <octet.h>
#include "munit/munit.h"

#define TEST_DATASET_PATH "./dataset/"

static MunitResult isProcessed_fromRawData_toFile(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;


    OctetData* trainingData = octet_prepare_training_data_from_dir(TEST_DATASET_PATH);


    munit_assert_int(trainingData->characterCount, ==, 4);

    char labels[4] = {'A', 'B', 'C', 'D'};

    for (int i = 0; i < trainingData->characterCount; i++) {
      munit_assert_char(trainingData->characters[i].label, ==, labels[i]);
    }

    octet_free_data(data);
    return MUNIT_OK;
}

MunitTest dataReadWriteTests[] = {
  { (char*) "/isProcessed_fromRawData_toFile", isProcessed_fromRawData_toFile, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
