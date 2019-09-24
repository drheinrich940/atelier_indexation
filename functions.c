//
// Created by Henri on 23/09/2019.
//
#include <stdio.h>
#include "functions.h"
#include "nrc/def.h"

void printMask(int mask[3][3]){
    for(int i = 0; i<3; i++){
        printf("\n");
        for(int j = 0; j<3; j++){
            printf(" %d ", mask[i][j]);
        }
    }
    printf("\n");
}

int verifyRGBValue(int value){
    if(value < -255)return -255;
    if(value > 255) return 255;
    if(value <200 && value >0) return 0;
    if(value <0 && value >-200) return 0;
    else return value;
}

void applyMaskToMatrix(int mask[3][3], byte **matrix, byte **outputMatrix, int matrix_max_x, int matrix_max_y){

    for(int i = 1; i<matrix_max_x-1; i++){
        for(int j = 1; j<matrix_max_y-1; j++){
            int temp = ( mask[0][0] * (int)matrix[i-1][j-1] + mask[0][1] * (int)matrix[i-1][j] + mask[0][2] * (int)matrix[i-1][j+1]
                         + mask[1][0] * (int)matrix[i][j-1] + mask[1][1] * (int)matrix[i][j] + mask[1][2] * (int)matrix[i][j+1]
                         + mask[2][0] * (int)matrix[i+1][j-1] + mask[2][1] * (int)matrix[i+1][j] + mask[2][2] * (int)matrix[i+1][j+1] )/9;
            printf("%d\n",temp);
            outputMatrix[i][j]=verifyRGBValue(temp);
        }
    }
}

void binariesPicture(byte **matrix, byte **output, int matrix_max_x, int matrix_max_y, int limit){
    for(int i = 1; i<matrix_max_x-1; i++){
        for(int j = 1; j<matrix_max_y-1; j++){
            if(matrix[i][j] > limit){
                output[i][j] = 255;
            }else{
                output[i][j] = 0;
            }
        }
    }
}

void greyScalesRGBPicture(rgb8 **matrix, byte **output, int matrix_max_x, int matrix_max_y){
    for(int i = 1; i<matrix_max_x-1; i++){
        for(int j = 1; j<matrix_max_y-1; j++){
            output[i][j] = (matrix[i][j].r + (int)matrix[i][j].g + (int)matrix[i][j].b)/3;
        }
    }
}

///
/// \param img l'image binariser
/// \param nrh nombre de ligne de l'image
/// \param nch nombre de colonne de l'image
/// \param histogramme histogramme de l'image
/// \return l'histogramme normaliser
void histogramme(byte** img,int nrh , int nch,double *histogramme){
    histogramme = malloc(256*sizeof(double));
    int max=nrh*nch;
    //initialise l'histogramme
    for(int i= 0 ; i < 256 ; i++){
        histogramme[i]=0.0;
    }

    //classifie l'image suivant les différents niveaux de gris
    for(int j=0; j < nrh ; j++){
        for(int k=0; k < nch ; k++){
            int index=img[j][k];
            histogramme[index]=histogramme[index]+1.0;
        }
    }
    //normalisation de l'histogramme
    for(int m= 0 ; m < 256 ; m++) histogramme[m]=histogramme[m]/(max*1.0);
}

/// Calcul la Distance entre 2 histogramme
/// \param hist1
/// \param hist2
/// \return distance de Bhattachryaa
double bhattacharyyaDistance(double* hist1,double* hist2){
    double distance=0.0;
    for(int i=0;i < 256 ; i++){
        distance+=sqrt(hist1[i]*hist2[i]);
    }
    distance=-log(distance);
    return distance;
}