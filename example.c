#include <octet.h>
#include <stdio.h> 

int main(void) {
    OctetData* trainingData = octet_load_training_data_from_dir("./dataset");
    if (!trainingData) return 1;

    OctetCharacter character = octet_load_character_from_image("./tests/test_data/test-A.jpg");
    char predictedLabel = octet_k_nearest_neighbour(character, trainingData, 1);

    octet_write_training_data_to_csv(trainingData, "data.csv");
    printf("Expected label: A\nPredicted label: %c\n", predictedLabel);

    octet_free_character(character);
    octet_free_training_data(trainingData);
    return 0;
}
