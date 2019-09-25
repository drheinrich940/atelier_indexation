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

void printMask(int mask[3][3]);
int verifyRGBValue(int value);
void applyMaskToMatrix(int mask[3][3], byte **matrix, byte **outputMatrix, int matrix_max_x, int matrix_max_y);
void binariesPicture(byte **matrix, byte **output, int matrix_max_x, int matrix_max_y, int limit);
void greyScalesRGBPicture(rgb8 **matrix, byte **output, int matrix_max_x, int matrix_max_y);
void histogramme(byte** img,int nrh , int nch,double *histogramme);
double bhattacharyyaDistance(double* hist1,double* hist2);
double tauxDeRouge(rgb8** img,int nrh , int nch);
double tauxDeBleu(rgb8** img,int nrh , int nch);
double tauxDeVert(rgb8** img,int nrh , int nch);
void normeGradient(byte** img,byte** gradient,int nrl, int nrh, int ncl, int nch);
double moyenneNormeGradient(byte** gradient,int nrh , int nch);
void border_detection_RGB (byte** inputImage, byte** gradientImage, long nrl, long nrh, long ncl, long nch);
void addTwoImages (byte** image1, byte** image2, byte** ImageSum, long nrl, long nrh, long ncl, long nch);
void sauvegardeHistogramme(char* nom,double* histogramme);

extern const int horizontal_gradient [3][3] ;
extern const int vertical_gradient[3][3];
#endif //ATELIER_INDEXATION_FUNCTIONS_H
