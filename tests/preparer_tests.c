#include <octet.h>
#include "munit/munit.h"

#define TEST_DATASET_PATH "./dataset/"
#define TEMP_DATA_PATH "./temp.csv"

static MunitResult PrepareDataFromDir_Dataset_ShouldProcessAndLoad(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    OctetData *trainingData = octet_load_training_data_from_dir(TEST_DATASET_PATH);
    munit_assert_not_null(trainingData);
    munit_assert_int32(trainingData->characterCount, ==, 8);

    char labels[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

    for (int i = 0; i < trainingData->characterCount; i++) {
        char c;
        for (int n = 0; n < (int)sizeof(labels); n++) {
            c = labels[n];
            if (c == trainingData->characters[i].label)
                break;
        }
        munit_assert_uint8(trainingData->characters[i].label, ==, c);
    }

    return MUNIT_OK;
}

static MunitResult WriteDataToFile_OctetData_ShouldWrite(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    OctetData *trainingData = octet_load_training_data_from_dir(TEST_DATASET_PATH);
    octet_write_training_data_to_csv(trainingData, TEMP_DATA_PATH);

    return MUNIT_OK;
}

static MunitResult ReadDataFromFile_LocalFile_ShouldReadAndMatch(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    OctetData *trainingData = octet_load_training_data_from_dir(TEST_DATASET_PATH);
    octet_write_training_data_to_csv(trainingData, TEMP_DATA_PATH);
    OctetData *trainingDataFromFile = octet_load_training_data_from_csv(TEMP_DATA_PATH);
    munit_assert_not_null(trainingDataFromFile);

    // Assuming you have 'trainingData' initialized and populated with data for comparison
    munit_assert_int(trainingDataFromFile->characterCount, ==, trainingData->characterCount);

    for (int i = 0; i < trainingDataFromFile->characterCount; i++) {
        OctetCharacter *charFromFile = &trainingDataFromFile->characters[i];
        OctetCharacter *charFromData = &trainingData->characters[i];

        munit_assert_int(charFromFile->width, ==, charFromData->width);
        munit_assert_int(charFromFile->height, ==, charFromData->height);
        munit_assert_int(charFromFile->label, ==, charFromData->label);
    }

    octet_free_training_data(trainingDataFromFile);
    octet_free_training_data(trainingData);

    return MUNIT_OK;
}

MunitTest dataReadWriteTests[] = {
    { (char*) "/PrepareDataFromDir_LocalDataset_ShouldLoad", PrepareDataFromDir_Dataset_ShouldProcessAndLoad, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/WriteOctetData_Local_CSV_ShouldWrite", WriteDataToFile_OctetData_ShouldWrite, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/ReadOctetData_Local_CSV_ShouldMatchMemory", ReadDataFromFile_LocalFile_ShouldReadAndMatch, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
