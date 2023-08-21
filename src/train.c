#include <octet/train.h>
#include <assert.h> 
#include <stdlib.h>
#include <math.h> // pow, sqrt

float octet_knn_calculate_distance_for_character(OctetCharacter trainingCharacter, OctetCharacter testingCharacter) {
  float finalDistance = 0;

  for (int i = 0; i < trainingCharacter.height; i++) {
    for (int j = 0; j < trainingCharacter.width; j++) {
      int difference = trainingCharacter.bytes[i * trainingCharacter.width + j] -  testingCharacter.bytes[i * trainingCharacter.width + j];
      finalDistance += pow(difference, 2);
    }
  }
  return sqrt(finalDistance);
}

int qsort_compare(const void *a, const void *b) {
  const OctetDistance *da = (const OctetDistance *)a;
  const OctetDistance *db = (const OctetDistance *)b;

  return (db->distance > da->distance) - (da->distance > db->distance);
}

char octet_k_nearest_neighbour(OctetData* trainingData, OctetCharacter testCharacter, const int k) {
  OctetDistance *distances = malloc(sizeof(OctetDistance) * trainingData->characterCount);
  int dCount = 0;

  for (int i = 0; i < trainingData->characterCount; i++) {
    OctetCharacter currentCharacter = trainingData->characters[i];

    OctetDistance currentDistance;
    currentDistance.distance = octet_knn_calculate_distance_for_character(currentCharacter, testCharacter);
    currentDistance.label = currentCharacter.label;

    distances[dCount] = currentDistance;
    dCount++;
  }

  qsort(distances, dCount, sizeof(OctetDistance), qsort_compare);

  int labelCounts[255];

  for (int i = 0; i < k; i++) {
    int neighborLabel = distances[i].label;
    labelCounts[neighborLabel]++;
  }

  int maxVotes = labelCounts[0];
  int predictedLabel = 0;

  for (int i = 1; i < 255; i++) {
    if (labelCounts[i] > maxVotes) {
      maxVotes = labelCounts[i];
      predictedLabel = i;
    }
  }


  free(distances);
  return predictedLabel;
}
