#include <octet/train.h>
#include <assert.h> 
#include <stdlib.h>
#include <math.h> // pow, sqrt
#include <string.h>

#include <stdio.h>

#define numOfLabels 128

float octet_knn_calculate_distance_for_character(OctetCharacter trainingCharacter, OctetCharacter testingCharacter) {
  double finalDistance = 0;

  for (int i = 0; i < trainingCharacter.height; i++) {
    for (int j = 0; j < trainingCharacter.width; j++) {
      int difference = abs(trainingCharacter.bytes[i * trainingCharacter.width + j] - testingCharacter.bytes[i * trainingCharacter.width + j]);
      finalDistance += difference * difference;
    }
  }
  return sqrt(finalDistance);
}

int qsort_compare(const void *a, const void *b) {
  const OctetDistance *da = (const OctetDistance *)a;
  const OctetDistance *db = (const OctetDistance *)b;

  if (da->distance < db->distance) {
    return -1;
  } else if (da->distance > db->distance) {
    return 1;
  } else {
    return 0;
  }
}

char octet_k_nearest_neighbour(OctetCharacter testCharacter, OctetData* trainingData, const int k) {
  OctetDistance *distances = malloc(sizeof(OctetDistance) * trainingData->characterCount);
  int dCount = 0;

  for (int i = 0; i < trainingData->characterCount; i++) {
    OctetCharacter currentCharacter = trainingData->characters[i];

    distances[dCount].label = currentCharacter.label;
    distances[dCount].distance = octet_knn_calculate_distance_for_character(currentCharacter, testCharacter);

    dCount++;
  }

  qsort(distances, dCount, sizeof(OctetDistance), qsort_compare);
  int labelCounts[numOfLabels] = {0};

  for (int i = 0; i < k; i++) {
    int neighborLabel = distances[i].label;
    labelCounts[neighborLabel]++;
  }


  int maxVotes = labelCounts[0];
  int predictedLabel = 0;

  for (int i = 1; i < numOfLabels; i++) {
    if (labelCounts[i] > maxVotes) {
      maxVotes = labelCounts[i];
      predictedLabel = i;
    }
  }

  free(distances);
  return predictedLabel;
}
