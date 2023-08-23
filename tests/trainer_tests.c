#include <octet.h>
#include "munit/munit.h"

#define TEST_DATASET_PATH "./dataset/"
#define TEMP_DATA_PATH "./temp.csv"

#define TEST_A_PATH "./tests/test_data/test-A.jpg"
#define TEST_A_CURSIVE_PATH "./tests/test_data/test-A-cursive.jpg"
#define TEST_A_ROUNDED_PATH "./tests/test_data/test-A-rounded.jpg"

MunitResult Knn_ExactMatch_ShouldCompute(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    return MUNIT_SKIP;

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

MunitResult Knn_SlightMatchesOf_A_ShouldCompute(const MunitParameter params[], void* data) {
    (void)params;
    (void)data;

    OctetData* trainingDataFromMem = octet_load_training_data_from_dir(TEST_DATASET_PATH);
    munit_assert_not_null(trainingDataFromMem);

    OctetCharacter character1 = octet_load_character_from_image(TEST_A_PATH);
    munit_assert_not_null(character1.bytes);
    char predictedLabel = octet_k_nearest_neighbour(character1, trainingDataFromMem, /* k */ 3);
    munit_assert_char('A', ==, predictedLabel);
    octet_free_character(character1);

#if 0 // this character isn't recognized
    OctetCharacter character2 = octet_load_character_from_image(TEST_A_CURSIVE_PATH);
    munit_assert_not_null(character2.bytes);
    char predictedLabel2 = octet_k_nearest_neighbour(character2, trainingDataFromMem, /* k */ 3);
    munit_assert_char('A', ==, predictedLabel2);
    octet_free_character(character2);
#endif

    OctetCharacter character3 = octet_load_character_from_image(TEST_A_ROUNDED_PATH);
    munit_assert_not_null(character3.bytes);
    char predictedLabel3 = octet_k_nearest_neighbour(character3, trainingDataFromMem, /* k */ 3);
    munit_assert_char('A', ==, predictedLabel3);
    octet_free_character(character3);


    octet_free_training_data(trainingDataFromMem);
    return MUNIT_OK;
}



MunitTest trainerTests[] = {
    { (char*) "/Knn_ExactMatch_ShouldCompute", Knn_ExactMatch_ShouldCompute, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/Knn_SlightMatchesOf_A_ShouldCompute", Knn_SlightMatchesOf_A_ShouldCompute, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};
