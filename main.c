#include<stdio.h>
#include<stdlib.h>

#include "nrc/def.h"
#include "nrc/nrio.h"
#include "nrc/nrarith.h"
#include "nrc/nralloc.h"
#include "masks.h"
#include "functions.h"


int main() {
    // Initialazing image parameters
    /*
     * nrl = row lowest value
     * ncl = column lowest value
     * nrh = row highest value
     * nch = column highest value
     */
    long *nrl, *nrh, *ncl, *nch;
    byte **imageTest;

    // Load an image into 2D matrix
    imageTest= LoadPGM_bmatrix("image1.pgm", &nrl, &nrh, &ncl, &nch);

    // Prints the chosen mask
    printMask(horizontal_gradient);

    byte **output;

    // Creating an empty matrix with our target dimensions.
    output = bmatrix(nrl, nrh, ncl, nch);
    printf("Created output matrix\n");

    // Applying the horisontal gradiant to the image test.
    // nrh is the max value on the rows, and nch is the max value on the columns.
    applyMaskToMatrix(horizontal_gradient, imageTest, output, nrh, nch);
    printf("Applied mask to matrix\n");

    // Save the matrix into another .pgm file.
    SavePGM_bmatrix(output, nrl, nrh, ncl, nch, "output.pgm") ;
    printf("Saved the output\n");

    // Clean the memory.
    free_bmatrix(imageTest, nrl,  nrh, ncl, nch) ;
    free_bmatrix(output, nrl,  nrh, ncl, nch) ;
    printf("Memory is free");
    return 0;
}