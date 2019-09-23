#include<stdio.h>
#include<stdlib.h>

#include "nrc/def.h"
#include "nrc/nrio.h"
#include "nrc/nrarith.h"
#include "nrc/nralloc.h"


int main() {
    // Initialazing image parameters
    long *nrl, *nrh, *ncl, *nch;
    byte **imageTest;

    //load an image into 2D matrix
    imageTest= LoadPGM_bmatrix("image1.pgm", &nrl, &nrh, &ncl, &nch);

    // Save the matrix into another .pgm file
    SavePGM_bmatrix(imageTest, nrl, nrh, ncl, nch, "imageTest.pgm") ;

    // Clean the memory
    free_bmatrix(imageTest, nrl,  nrh, ncl, nch) ;
    return 0;
}