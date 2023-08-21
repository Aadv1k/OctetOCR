#include <octet.h>
#include "munit/munit.h"

#define TEST_DATASET_PATH "./dataset/"
#define TEMP_DATA_PATH "./temp.csv"

MunitResult Knn_ExactMatch_ShouldCompute(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    OctetData* trainingDataFromMem = octet_load_training_data_from_dir(TEST_DATASET_PATH);
    munit_assert_not_null(trainingDataFromMem);

    char possibleMatch = octet_k_nearest_neighbour(trainingDataFromMem->characters[0], trainingDataFromMem, 3);
    char possibleMatch2 = octet_k_nearest_neighbour(trainingDataFromMem->characters[1], trainingDataFromMem, 3);

    munit_assert_char('A', ==, possibleMatch);
    munit_assert_char('B', ==, trainingDataFromMem->characters[1].label);
    munit_assert_char('D', !=, trainingDataFromMem->characters[2].label);

    octet_free_training_data(trainingDataFromMem);
    return MUNIT_OK;
}

MunitResult Knn_SlightMatch_ShouldCompute(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    return MUNIT_SKIP;
}



MunitTest trainerTests[] = {
    { (char*) "/Knn_ExactMatch_ShouldCompute", Knn_ExactMatch_ShouldCompute, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/Knn_SlightMatch_ShouldCompute", Knn_SlightMatch_ShouldCompute, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
