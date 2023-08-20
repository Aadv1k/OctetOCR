#ifndef _OCTET_TRAIN_H
#define _OCTET_TRAIN_H

#include <octet/prep.h>

void octet_k_nearest_neighbour(OctetData* trainingData, OctetData* testingData, const int k);

int octet_knn_calculate_distance(OctetData* trainingData, OctetCharacter character);

#endif // _OCTET_TRAIN_H

