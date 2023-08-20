#include <octet.h>
#include "munit/munit.h"

#define TEST_DATASET_PATH "./dataset/"
#define TEMP_DATA_PATH "./temp.csv"

static OctetData *trainingData;

static MunitResult PrepareDataFromDir_Dataset_ShouldProcessAndLoad(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    trainingData = octet_load_training_data_from_dir(TEST_DATASET_PATH);
    munit_assert_not_null(trainingData);
    munit_assert_int32(trainingData->characterCount, ==, 4);

    char labels[4] = {'A', 'B', 'C', 'D'};

    for (int i = 0; i < trainingData->characterCount; i++) {
        munit_assert_uint8(trainingData->characters[i].label, ==, labels[i]);
    }

    return MUNIT_OK;
}

static MunitResult WriteDataToFile_OctetData_ShouldWrite(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    octet_write_training_data_to_csv(trainingData, TEMP_DATA_PATH);

    return MUNIT_OK;
}

extern MunitTest dataReadWriteTests[] = {
    { (char*) "/PrepareDataFromDir_Dataset_ShouldProcessAndLoad", PrepareDataFromDir_Dataset_ShouldProcessAndLoad, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/WriteDataToFile_OctetData_ShouldWrite", WriteDataToFile_OctetData_ShouldWrite, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
