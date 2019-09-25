//
// Created by Henri on 23/09/2019.
//
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <dirent.h>
#include<string.h>
#include "functions.h"
#include "nrc/def.h"
#include ".\nrc\nrio.h"
#include ".\nrc\nrarith.h"
#include ".\nrc\nralloc.h"

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
    // Edit value to 15 or 20 to have absolute 255 pixel value edge detection
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
void applyMaskToMatrix(int mask[3][3], byte **matrix, byte **outputMatrix, int matrix_max_x, int matrix_max_y) {

    for (int i = 1; i < matrix_max_x - 1; i++) {
        for (int j = 1; j < matrix_max_y - 1; j++) {
            int temp = (mask[0][0] * (int) matrix[i - 1][j - 1] + mask[0][1] * (int) matrix[i - 1][j] +
                        mask[0][2] * (int) matrix[i - 1][j + 1]
                        + mask[1][0] * (int) matrix[i][j - 1] + mask[1][1] * (int) matrix[i][j] +
                        mask[1][2] * (int) matrix[i][j + 1]
                        + mask[2][0] * (int) matrix[i + 1][j - 1] + mask[2][1] * (int) matrix[i + 1][j] +
                        mask[2][2] * (int) matrix[i + 1][j + 1]) / 9;
            outputMatrix[i][j] = verifyRGBValue(temp);
        }
    }
}

/**
 *
 * @param mask
 * @param matrix
 * @param outputMatrix
 * @param matrix_max_x
 * @param matrix_max_y
 */
void applyMaskToMatrix_bounded(const int mask[3][3], byte **matrix, byte **outputMatrix, int nrl, int nrh, int ncl,
                               int nch) {
    for (int i = nrl + 1; i < nrh - 1; i++) {
        for (int j = ncl + 1; j < nch - 1; j++) {
            int pixel_value = floor((mask[0][0] * matrix[i - 1][j - 1] + mask[0][1] * matrix[i - 1][j] +
                                     mask[0][2] * matrix[i - 1][j + 1]
                                     + mask[1][0] * matrix[i][j - 1] + mask[1][1] * matrix[i][j] +
                                     mask[1][2] * matrix[i][j + 1]
                                     + mask[2][0] * matrix[i + 1][j - 1] + mask[2][1] * matrix[i + 1][j] +
                                     mask[2][2] * matrix[i + 1][j + 1]) / 9);
            outputMatrix[i][j] = verifyRGBValue(pixel_value);

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
void detectionBords(byte **img, byte **output, long threshold, long nrl, long nrh, long ncl, long nch) {
    byte **deriv_x, **deriv_y;
    long normeGradient;
    deriv_x = bmatrix(nrl, nrh, ncl, nch);
    deriv_y = bmatrix(nrl, nrh, ncl, nch);
    applyMaskToMatrix_bounded(horizontal_gradient, img, deriv_x, nrl, nrh, ncl, nch);
    applyMaskToMatrix_bounded(vertical_gradient, img, deriv_y, nrl, nrh, ncl, nch);
    for (int i = 1; i < nrh - nrl; i++) {
        for (int j = 1; j < nch - ncl; j++) {
            normeGradient = sqrt(pow(deriv_x[i][j], 2) + pow(deriv_y[i][j], 2));
            if (normeGradient >= threshold) {
                output[i][j] = normeGradient;
            } else {
                output[i][j] = 0;
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
void normeGradient(byte **img, byte **output, long nrl, long nrh, long ncl, long nch) {
    byte **deriv_x, **deriv_y;
    deriv_x = bmatrix(nrl, nrh, ncl, nch);
    deriv_y = bmatrix(nrl, nrh, ncl, nch);
    applyMaskToMatrix_bounded(horizontal_gradient, img, deriv_x, nrl, nrh, ncl, nch);
    applyMaskToMatrix_bounded(vertical_gradient, img, deriv_y, nrl, nrh, ncl, nch);
    for (int i = 1; i < nrh - nrl; i++) {
        for (int j = 1; j < nch - ncl; j++) {
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
double moyenneNormeGradient(byte **gradient, int nrh, int nch) {
    double moyenne = 0.0;
    for (int i = 0; i < nrh; i++) {
        for (int j = 0; j < nch; j++) {
            moyenne += gradient[i][j];

        }
    }
    moyenne = moyenne / (nrh * nch);
    return moyenne;
}

/**
 * Sauvegarde d'un histogramme
 * @param nom nom du fichier
 * @param histogramme histogramme a enregistré
 */
void sauvegardeHistogramme( double *histogramme,FILE* f) {
    for (int i = 0; i < 256; i++) {
        fprintf(f, "%lf;", histogramme[i]);
    }
}


/**
 *
 * @param img
 * @param nrh
 * @param nch
 * @return 1 si l'image est coloré ou 0 si l'image est en noir est blanc
 */
int colored(rgb8** img,int nrh , int nch){
    int i=0;
    int j=0;
    int color=0;
    for(int i=0;i < nrh ; i++) {
        for (int j = 0; j < nch; j++) {
            if (img[i][j].r != img[i][j].b || img[i][j].r != img[i][j].g)
                color=1;
        }
    }
    return color;
}


int lectureDossier(char *nomdossier){
    DIR* rep =NULL;
    FILE* f;
    struct dirent* currentImg=NULL;
    f=fopen("..\\output.csv","wa");
    if(f == NULL){
        printf("Error open output file");
        return 1;
    }
    // ouverture du dossier
    rep = opendir(nomdossier);
    if(rep== NULL) {
        printf("Error read directory");
        return 1;
    }
    //lecture des dossier
    currentImg = readdir(rep);
    currentImg = readdir(rep);
// lecture des images
    while ((currentImg = readdir(rep)) != NULL){
        //vérification du type
        char imgName[255];
        sprintf(imgName,"%s\\%s",nomdossier,currentImg->d_name);
        char* imgType=NULL;
        //récupération de l'extension
        imgType=strtok(currentImg->d_name,".");
        imgType=strtok(NULL,".");
        if(!strcmp(imgType,"ppm")){
            double tauxR,tauxG,tauxB=0.0;
            double texture=0.0;
            double moyenneGradient=0.0;

            int color=0;
            int nbPixelContour=0;
            rgb8** image;
            byte** imageBW;
            byte** gradient;
            long nrl,nrh,ncl,nch;
            image=LoadPPM_rgb8matrix(imgName, &nrl, &nrh, &ncl, &nch);
            imageBW=bmatrix(nrl, nrh, ncl, nch);
            gradient=bmatrix(nrl, nrh, ncl, nch);
            color=colored(image,nrh,nch);
            greyScalesRGBPicture(image,imageBW,nrh,nch);
            double *hist= malloc(256 * sizeof(double));
            histogramme(imageBW,nrh,nch,hist);
            for(int i=0;i < 256 ; i++)
                printf("out=%lf\n",hist[i]);
            if(color){
                tauxR=tauxDeRouge(image,nrh,nch);
                tauxG=tauxDeVert(image,nrh,nch);
                tauxB=tauxDeBleu(image,nrh,nch);
            }
            else{
                tauxR=0.33;
                tauxG=0.33;
                tauxB=0.33;
            }
            texture=0;
            normeGradient(image,gradient,nrl,nrh,ncl,nch);
            moyenneGradient=moyenneNormeGradient(gradient,nrh,nch);
            fprintf(f,"%d;%d;%lf;%lf;%lf;%lf",color,nbPixelContour,tauxR,tauxG,tauxB,moyenneGradient);
            sauvegardeHistogramme(hist,f);
            fprintf(f,"\n");
            free_bmatrix(image, nrl,  nrh, ncl, nch) ;
            free_bmatrix(gradient, nrl,  nrh, ncl, nch) ;
            free(hist);
        }
    }
    fclose(f);
    if (closedir(rep) == -1){
        printf("Error close directory");
        return 1;
    }
    return 0;
}
