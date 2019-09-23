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