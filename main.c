#include<stdio.h>

#include "nrc/def.h"
#include "nrc/nrio.h"
#include "nrc/nralloc.h"
#include "functions.h"
#include "masks.h"


int main() {
    // Initialazing image parameters
    /*
     * nrl = row lowest value
     * ncl = column lowest value
     * nrh = row highest value
     * nch = column highest value
     * matrix[row][column]
     */
    long nrl, nrh, ncl, nch;
    byte **imageTest;
    double moyenneGradient;
    long nbPixelBord;

    // Load an image into 2D matrix
    imageTest = LoadPGM_bmatrix("rice.pgm", &nrl, &nrh, &ncl, &nch);

    // Prints the chosen mask
    printMask(horizontal_gradient);

    //byte **output;
    byte **output_fromBounded;

    // Creating an empty matrix with our target dimensions.
    //output = bmatrix(nrl, nrh, ncl, nch);
    output_fromBounded = bmatrix(nrl, nrh, ncl, nch);
    initMatrix(output_fromBounded, nrh, nch);
    printf("Created output matrix\n");
    printf("%lu %lu \n", nrh, nch);


    // Applying the horisontal gradient to the image test.
    // nrh is the max value on the rows, and nch is the max value on the columns.
    //applyMaskToMatrix_bounded(vertical_gradient, imageTest, output_fromBounded, nrl, nrh, ncl, nch);
    //applyMaskToMatrix(horizontal_gradient, imageTest, output, nrh, nch);
    detectionBords(imageTest, output_fromBounded,30, &moyenneGradient,&nbPixelBord, nrl, nrh, ncl, nch);
    printf("Applied mask to matrix\n");

    // Gradient mean calculation

    printf("Moyenne gradient : %f\n",moyenneGradient );
    printf("Nombre de Pixels de contour : %d\n",nbPixelBord );
    // Save the matrix into another .pgm file.
    // SavePGM_bmatrix(output, nrl, nrh, ncl, nch, "output.pgm");
    SavePGM_bmatrix(output_fromBounded, nrl, nrh, ncl, nch, "output_fromBounded.pgm");
    printf("Saved the output\n");

    // Clean the memory.
    free_bmatrix(imageTest, nrl, nrh, ncl, nch);
   // free_bmatrix(output, nrl, nrh, ncl, nch);
    free_bmatrix(output_fromBounded, nrl, nrh, ncl, nch);
    printf("Memory is free");
    return 0;
}