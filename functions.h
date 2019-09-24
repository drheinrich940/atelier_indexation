//
// Created by Henri on 23/09/2019.
//

#ifndef ATELIER_INDEXATION_FUNCTIONS_H
#define ATELIER_INDEXATION_FUNCTIONS_H

#include "nrc/def.h"

/*
 * PrintMask allows you to print the selected mask in the console.
 * VerifyMask allows you to post process and verify that the value from the mask convolution is correct.
 * ApplyMastToMatrix applies the selected 3*3 mask to the targeted matrix.
 * Binarises replaces each pixel by a white or a black one in function of its value.
 */

//TODO : FREE WHAT SHOULD BE


void binariesPicture(byte **matrix, byte **output, int matrix_max_x, int matrix_max_y, int limit);
void greyScalesRGBPicture(rgb8 **matrix, byte **output, int matrix_max_x, int matrix_max_y);
void histogramme(byte** img,int nrh , int nch,double *histogramme);
double bhattacharyyaDistance(double* hist1,double* hist2);
double tauxDeRouge(rgb8** img,int nrh , int nch);
double tauxDeBleu(rgb8** img,int nrh , int nch);
double tauxDeVert(rgb8** img,int nrh , int nch);
#endif //ATELIER_INDEXATION_FUNCTIONS_H
