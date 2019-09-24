//
// Created by Henri on 24/09/2019.
//

#ifndef ATELIER_INDEXATION_CONVOLUTIONS_H
#define ATELIER_INDEXATION_CONVOLUTIONS_H
#include "../nrc/def.h"


void printMask(int mask[3][3]);
int verifyRGBValue(int value);
void applyMaskToMatrix(int mask[3][3], byte **matrix, byte **outputMatrix, int matrix_max_x, int matrix_max_y);

#endif //ATELIER_INDEXATION_CONVOLUTIONS_H
