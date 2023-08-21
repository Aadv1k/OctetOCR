#ifndef _OCTET_TRAIN_H
#define _OCTET_TRAIN_H

#include <octet/prep.h>

typedef struct {
  char label;
  float distance;
} OctetDistance;

float octet_knn_calculate_distance_for_character(OctetCharacter trainingCharacter, OctetCharacter testingCharacter);
int qsort_compare(const void *a, const void *b);
char octet_k_nearest_neighbour(OctetCharacter testCharacter, OctetData* trainingData, const int k);

#endif // _OCTET_TRAIN_H

