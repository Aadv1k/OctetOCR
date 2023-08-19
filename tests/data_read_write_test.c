#include <octet.h>
#include "munit/munit.h"

#define TEST_DATASET_PATH "./dataset/"
#define TEMP_DATA_PATH "./temp.dat"

static OctetData* trainingData;
static OctetData* trainingDataB;

static MunitResult PrepareDataFromDir_Dataset_ShouldProcessAndLoad(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    trainingData = octet_prepare_training_data_from_dir(TEST_DATASET_PATH);
    munit_assert_int(trainingData->characterCount, ==, 4);

    char labels[4] = {'A', 'B', 'C', 'D'};

    for (int i = 0; i < trainingData->characterCount; i++) {
        munit_assert_char(trainingData->characters[i].label, ==, labels[i]);
    }
    return MUNIT_OK;
}

static MunitResult WriteDataToFile_OctetData_ShouldWrite(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    octet_dump_training_data_to_file(trainingData, TEMP_DATA_PATH);

    // You need to add assertions here to check if the TEMP_DATA_PATH was created and exists
    // Example assertions: munit_assert_int(access(TEMP_DATA_PATH, F_OK), ==, 0);

    return MUNIT_OK;
}

static MunitResult LoadDataFromFile_TempDataPath_ShouldRead(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;


    trainingDataB = octet_read_training_data_from_file(TEMP_DATA_PATH);
    munit_assert_ptr_not_null(trainingDataB);


    munit_assert_int(trainingDataB->characterCount, ==, trainingData->characterCount);

    for (int i = 0; i < trainingDataB->characterCount; i++) {
        munit_assert_char(trainingDataB->characters[i].label, ==, trainingData->characters[i].label);
        munit_assert_int(trainingDataB->characters[i].width, ==, trainingData->characters[i].width);
        munit_assert_int(trainingDataB->characters[i].height, ==, trainingData->characters[i].height);

        // Compare image bytes if needed
    }

    return MUNIT_OK;
}

static void test_teardown(void* fixture) {
    (void)fixture;
    octet_free_data(trainingData);
    octet_free_data(trainingDataB);
}

MunitTest dataReadWriteTests[] = {
  { (char*) "/PrepareDataFromDir_Dataset_ShouldProcessAndLoad", PrepareDataFromDir_Dataset_ShouldProcessAndLoad, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/WriteDataToFile_OctetData_ShouldWrite", WriteDataToFile_OctetData_ShouldWrite, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/LoadDataFromFile_TempDataPath_ShouldRead", LoadDataFromFile_TempDataPath_ShouldRead, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
