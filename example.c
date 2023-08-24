#include <octet.h>
#include <stdio.h> 

int main(void) {
    /* OctetData* trainingData = octet_load_training_data_from_dir("./dataset"); */
    OctetData *trainingData = octet_load_training_data_from_csv("./data.csv");
    if (!trainingData) return 1;

    for (int i = 0; i < trainingData->characterCount; i++) {
        printf("%c: width %d height %d\n", trainingData->characters[i].label,
                trainingData->characters[i].width, trainingData->characters[i].height);
        printf("\tnine random characters: %d %d %d %d %d %d %d %d %d\n",
                trainingData->characters[i].bytes[0],
                trainingData->characters[i].bytes[10],
                trainingData->characters[i].bytes[40],
                trainingData->characters[i].bytes[80],
                trainingData->characters[i].bytes[120],
                trainingData->characters[i].bytes[160],
                trainingData->characters[i].bytes[200],
                trainingData->characters[i].bytes[300],
                trainingData->characters[i].bytes[400]);
    }

    OctetCharacter character = octet_load_character_from_image("./tests/test_data/test-A.jpg");
    char predictedLabel = octet_k_nearest_neighbour(character, trainingData, 1);

    octet_write_training_data_to_csv(trainingData, "data.csv");
    printf("Expected label: A\nPredicted label: %c\n", predictedLabel);

    octet_free_character(character);
    octet_free_training_data(trainingData);
    return 0;
}
