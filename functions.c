//
// Created by Henri on 23/09/2019.
//
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "functions.h"
#include "nrc/def.h"
#include "nrc/nralloc.h"



/**
 *
 * @param mask
 */
void printMask(const int mask[3][3]) {
    for (int i = 0; i < 3; i++) {
        printf("\n");
        for (int j = 0; j < 3; j++) {
            printf(" %d ", mask[i][j]);
        }
    }
    printf("\n");
}

/**
 *
 * @param value
 * @return
 */
int verifyRGBValue(int value) {
    if (value < 0)return 0;
    if (value > 255) return 255;
    else return value;
}

/**
 *
 * @param mask
 * @param matrix
 * @param outputMatrix
 * @param matrix_max_x
 * @param matrix_max_y
 */
void applyMaskToMatrix(const int mask[3][3], byte **matrix, byte **outputMatrix, int nrl, int nrh, int ncl, int nch) {
    for (int i = nrl+1; i < nrh - 1; i++) {
        for (int j =ncl+ 1; j < nch - 1; j++) {
                int pixel_value  = floor((mask[0][0] * matrix[i - 1][j - 1] + mask[0][1] *  matrix[i - 1][j] +
                        mask[0][2] *  matrix[i - 1][j + 1]
                        + mask[1][0] *  matrix[i][j - 1] + mask[1][1] *  matrix[i][j] +
                        mask[1][2] *  matrix[i][j + 1]
                        + mask[2][0] *  matrix[i + 1][j - 1] + mask[2][1] *  matrix[i + 1][j] +
                        mask[2][2] *  matrix[i + 1][j + 1]) / 9);
            outputMatrix[i][j]= verifyRGBValue(pixel_value);

        }
    }
}

/**
 *
 * @param matrix
 * @param output
 * @param matrix_max_x
 * @param matrix_max_y
 * @param limit
 */
void binariesPicture(byte **matrix, byte **output, int matrix_max_x, int matrix_max_y, int limit) {
    for (int i = 1; i < matrix_max_x - 1; i++) {
        for (int j = 1; j < matrix_max_y - 1; j++) {
            if (matrix[i][j] > limit) {
                output[i][j] = 255;
            } else {
                output[i][j] = 0;
            }
        }
    }
}

/**
 *
 * @param matrix
 * @param output
 * @param matrix_max_x
 * @param matrix_max_y
 */
void greyScalesRGBPicture(rgb8 **matrix, byte **output, int matrix_max_x, int matrix_max_y) {
    for (int i = 1; i < matrix_max_x - 1; i++) {
        for (int j = 1; j < matrix_max_y - 1; j++) {
            output[i][j] = (matrix[i][j].r + (int) matrix[i][j].g + (int) matrix[i][j].b) / 3;
        }
    }
}


///
/// \param img l'image binariser
/// \param nrh nombre de ligne de l'image
/// \param nch nombre de colonne de l'image
/// \param histogramme histogramme de l'image
/// \return l'histogramme normaliser
void histogramme(byte **img, int nrh, int nch, double *histogramme) {
    histogramme = malloc(256 * sizeof(double));
    int max = nrh * nch;
    //initialise l'histogramme
    for (int i = 0; i < 256; i++) {
        histogramme[i] = 0.0;
    }

    //classifie l'image suivant les différents niveaux de gris
    for (int j = 0; j < nrh; j++) {
        for (int k = 0; k < nch; k++) {
            int index = img[j][k];
            histogramme[index] = histogramme[index] + 1.0;
        }
    }
    //normalisation de l'histogramme
    for (int m = 0; m < 256; m++) histogramme[m] = histogramme[m] / (max * 1.0);
}

/// Calcul la Distance entre 2 histogramme
/// \param hist1
/// \param hist2
/// \return distance de Bhattachryaa
double bhattacharyyaDistance(double *hist1, double *hist2) {
    double distance = 0.0;
    for (int i = 0; i < 256; i++) {
        distance += sqrt(hist1[i] * hist2[i]);
    }
    distance = -log(distance);
    return distance;
}

/// Calcul le Taux de rouge dans une image
/// \param img image coloré rgb
/// \param nrh nombre de lignes
/// \param nch nombre de colonnes
/// \return le taux de rouge d'une image
double tauxDeRouge(rgb8 **img, int nrh, int nch) {
    double tauxr = 0.0;
    double tauxg = 0.0;
    double tauxb = 0.0;
    for (int i = 0; i < nrh; i++) {
        for (int j = 0; j < nch; j++) {
            tauxr += img[i][j].r;
            tauxb += img[i][j].b;
            tauxg += img[i][j].g;
        }
    }
    tauxr = tauxr / (tauxr + tauxb + tauxg);
    return tauxr;
}

/// Calcul le Taux de bleu dans une image
/// \param img image coloré rgb
/// \param nrh nombre de lignes
/// \param nch nombre de colonnes
/// \return le taux de bleu d'une image
double tauxDeBleu(rgb8 **img, int nrh, int nch) {
    double tauxr = 0.0;
    double tauxg = 0.0;
    double tauxb = 0.0;
    for (int i = 0; i < nrh; i++) {
        for (int j = 0; j < nch; j++) {
            tauxr += img[i][j].r;
            tauxb += img[i][j].b;
            tauxg += img[i][j].g;
        }
    }
    tauxb = tauxb / (tauxr + tauxb + tauxg);
    return tauxb;
}

/// Calcul le Taux de vert dans une image
/// \param img image coloré rgb
/// \param nrh nombre de lignes
/// \param nch nombre de colonnes
/// \return le taux de vert d'une image
double tauxDeVert(rgb8 **img, int nrh, int nch) {
    double tauxr = 0.0;
    double tauxg = 0.0;
    double tauxb = 0.0;
    for (int i = 0; i < nrh; i++) {
        for (int j = 0; j < nch; j++) {
            tauxr += img[i][j].r;
            tauxb += img[i][j].b;
            tauxg += img[i][j].g;
        }
    }
    tauxg = tauxg / (tauxr + tauxb + tauxg);
    return tauxg;
}


/**
 * Additionne deux matrices de tailles identiques.
 * @param image1
 * @param image2
 * @param ImageSum
 * @param nrl
 * @param nrh
 * @param ncl
 * @param nch
 */
void addTwoImages(byte **image1, byte **image2, byte **ImageSum, long nrl, long nrh, long ncl, long nch) {
    long x_border = nch - ncl;
    long y_border = nrh - nrl;

    for (long i = 0; i <= x_border; i++) {
        for (long j = 0; j <= y_border; j++) {
            ImageSum[i][j] = image1[i][j] + image2[i][j];
        }
    }
}

/**
 *
 * @param inputImage
 * @param gradientImage
 * @param nrl
 * @param nrh
 * @param ncl
 * @param nch
 */
void detectionBords(byte** img, byte** output, long threshold, long nrl , long nrh,long ncl,long nch) {
    byte **deriv_x, **deriv_y;
    long normeGradient;
    deriv_x = bmatrix(nrl, nrh, ncl, nch);
    deriv_y = bmatrix(nrl, nrh, ncl, nch);
    applyMaskToMatrix(horizontal_gradient, img, deriv_x, nrl, nrh, ncl, nch);
    applyMaskToMatrix(vertical_gradient, img, deriv_y, nrl, nrh, ncl, nch);
    for (int i = 1; i < nrh - nrl ; i++) {
        for (int j = 1; j < nch-ncl; j++) {
            normeGradient=sqrt(pow(deriv_x[i][j], 2) + pow(deriv_y[i][j], 2));
            if (normeGradient>=threshold){
                output[i][j] = normeGradient;}
            else{
                output[i][j]=0;
            }
        }
    }
}

/**
 *
 * @param img
 * @param gradient
 * @param nrh
 * @param nch
 * @param nrl
 * @param ncl
 */
void normeGradient(byte** img, byte** output, long nrl , long nrh,long ncl,long nch) {
    byte **deriv_x, **deriv_y;
    deriv_x = bmatrix(nrl, nrh, ncl, nch);
    deriv_y = bmatrix(nrl, nrh, ncl, nch);
    applyMaskToMatrix(horizontal_gradient, img, deriv_x, nrl, nrh, ncl, nch);
    applyMaskToMatrix(vertical_gradient, img, deriv_y, nrl, nrh, ncl, nch);
    for (int i = 1; i < nrh - nrl ; i++) {
        for (int j = 1; j < nch-ncl; j++) {
            output[i][j] = sqrt(pow(deriv_x[i][j], 2) + pow(deriv_y[i][j], 2));
        }
    }
}
/**
 *
 * @param gradient
 * @param nrh
 * @param nch
 * @return
 */
double moyenneNormeGradient(byte** gradient,int nrh , int nch){
    double moyenne= 0.0;
    for (int i = 0; i < nrh; i++) {
        for (int j = 0; j < nch; j++) {
            moyenne +=gradient[i][j];

        }
    }
    moyenne=moyenne/(nrh*nch);
    return  moyenne;
}

/**
 * Sauvegarde d'un histogramme
 * @param nom nom du fichier
 * @param histogramme histogramme a enregistré
 */
void sauvegardeHistogramme(char* nom,double* histogramme){
    FILE * f=fopen(nom,"wa");
    for(int i =0 ; i < 256 ; i++){
        fprintf(f,"%lf \n",histogramme);
    }
}
