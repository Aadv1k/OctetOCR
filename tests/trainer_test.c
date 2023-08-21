#include <octet.h>
#include "munit/munit.h"

#define TEST_DATASET_PATH "./dataset/"
#define TEMP_DATA_PATH "./temp.csv"

static OctetData *trainingData;

MunitResult Knn_Dataset_ShouldComputeWithoutFail(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    OctetData* trainingData = octet_load_training_data_from_dir(TEST_DATASET_PATH);
    munit_assert_not_null(trainingData);

    octet_free_training_data(trainingData);
    return MUNIT_OK;
}

MunitTest trainerTests[] = {
    { (char*) "/Knn_Dataset_ShouldComputeWithoutFail", Knn_Dataset_ShouldComputeWithoutFail, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
