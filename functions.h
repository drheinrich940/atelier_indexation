//
// Created by Henri on 23/09/2019.
//

#ifndef ATELIER_INDEXATION_FUNCTIONS_H
#define ATELIER_INDEXATION_FUNCTIONS_H

#include "nrc/def.h"

/*
 * PrintMask allows you to print the selected mask in the console. yeepee !
 * VerifyMask allows you to post process and verify that the value from the mask convolution is correct.
 * ApplyMastToMatrix applies the selected 3*3 mask to the targetted matrix.
 */

void printMask(int mask[3][3]);
int verifyRGBValue(int value);
void applyMaskToMatrix(int mask[3][3], byte **matrix, byte **outputMatrix, int matrix_max_x, int matrix_max_y);
#endif //ATELIER_INDEXATION_FUNCTIONS_H
